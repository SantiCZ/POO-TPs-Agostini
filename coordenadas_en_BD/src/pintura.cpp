#include "pintura.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>

Pintura::Pintura(AdminDB *adminDB, QWidget *parent)
    : QWidget(parent)
    , m_adminDB(adminDB)
    , m_color(Qt::black)
    , m_grosor(3)
    , m_dibujando(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_StaticContents);
    setCursor(Qt::CrossCursor);
    setMinimumSize(400, 300);
}

// ── carga inicial desde bd ────────────────────────────────────────
void Pintura::cargarDesdeDB()
{
    if (!m_adminDB || !m_adminDB->getDB().isOpen()) return;

    m_historial = m_adminDB->cargarTrazos();
    redibujarDesdeHistorial();

    emit statusMessage(
        QString("Trazos cargados: %1 segmentos").arg(m_historial.size())
        );
}

// ── setters ───────────────────────────────────────────────────────
void Pintura::setColor(const QColor &c)
{
    m_color = c;
    emit colorCambiado(c);
}

void Pintura::setGrosor(int g)
{
    m_grosor = qBound(1, g, 50);
    emit grosorCambiado(m_grosor);
}

// ── redibuja el canvas desde el historial en memoria ──────────────
void Pintura::redibujarDesdeHistorial()
{
    if (m_canvas.isNull()) return;

    m_canvas.fill(Qt::white);
    QPainter p(&m_canvas);
    p.setRenderHint(QPainter::Antialiasing);

    for (const auto &seg : m_historial) {
        if (seg.puntos.size() < 2) continue;

        QPen pen(seg.color);
        pen.setWidth(seg.grosor);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);

        for (int i = 1; i < seg.puntos.size(); ++i)
            p.drawLine(seg.puntos[i-1], seg.puntos[i]);
    }
    update();
}

// ── dibuja un segmento de linea sobre el buffer ───────────────────
void Pintura::dibujarSegmento(const QPoint &desde, const QPoint &hasta)
{
    QPainter p(&m_canvas);
    p.setRenderHint(QPainter::Antialiasing);

    QPen pen(m_color);
    pen.setWidth(m_grosor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    p.drawLine(desde, hasta);

    int margen = m_grosor + 2;
    QRect rect = QRect(desde, hasta).normalized()
                     .adjusted(-margen, -margen, margen, margen);
    update(rect);
}

// ── deshacer ──────────────────────────────────────────────────────
void Pintura::deshacerUltimo()
{
    if (m_pilaDeshacer.isEmpty()) {
        emit statusMessage("Nada que deshacer");
        return;
    }

    int segId = m_pilaDeshacer.takeLast();

    // borrar en bd
    if (m_adminDB && m_adminDB->getDB().isOpen())
        m_adminDB->borrarSegmento(segId);

    // borrar en memoria
    for (int i = m_historial.size() - 1; i >= 0; --i) {
        if (m_historial[i].id == segId) {
            m_historial.removeAt(i);
            break;
        }
    }

    redibujarDesdeHistorial();
    emit statusMessage(
        QString("Deshacer — quedan %1 trazos").arg(m_historial.size())
        );
}

// ── borrar todo ───────────────────────────────────────────────────
void Pintura::borrarTodo()
{
    m_historial.clear();
    m_pilaDeshacer.clear();

    if (m_adminDB && m_adminDB->getDB().isOpen())
        m_adminDB->borrarTodosTrazos();

    m_canvas.fill(Qt::white);
    update();
    emit statusMessage("Lienzo borrado");
}

// ── paintevent ────────────────────────────────────────────────────
void Pintura::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    if (!m_canvas.isNull())
        p.drawPixmap(event->rect(), m_canvas, event->rect());
}

// ── resizeevent ───────────────────────────────────────────────────
void Pintura::resizeEvent(QResizeEvent *event)
{
    QPixmap nuevo(event->size());
    nuevo.fill(Qt::white);
    if (!m_canvas.isNull()) {
        QPainter p(&nuevo);
        p.drawPixmap(0, 0, m_canvas);
    }
    m_canvas = nuevo;
    redibujarDesdeHistorial();
    QWidget::resizeEvent(event);
}

// ── mousepressevent ───────────────────────────────────────────────
void Pintura::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;

    m_dibujando   = true;
    m_ultimoPunto = event->pos();

    // iniciar nuevo segmento en bd
    m_trazoActivo = TrazoActivo();
    m_trazoActivo.color  = m_color;
    m_trazoActivo.grosor = m_grosor;
    m_trazoActivo.puntos.append(event->pos());

    if (m_adminDB && m_adminDB->getDB().isOpen()) {
        m_trazoActivo.segmentoId = m_adminDB->guardarSegmento(m_color, m_grosor);
        if (m_trazoActivo.segmentoId > 0)
            m_adminDB->agregarPunto(m_trazoActivo.segmentoId,
                                    event->pos(),
                                    m_trazoActivo.contadorPuntos++);
    }
}

// ── mousemoveevent ────────────────────────────────────────────────
void Pintura::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dibujando || !(event->buttons() & Qt::LeftButton)) return;

    QPoint actual = event->pos();
    dibujarSegmento(m_ultimoPunto, actual);

    m_trazoActivo.puntos.append(actual);

    if (m_adminDB && m_adminDB->getDB().isOpen() && m_trazoActivo.segmentoId > 0)
        m_adminDB->agregarPunto(m_trazoActivo.segmentoId,
                                actual,
                                m_trazoActivo.contadorPuntos++);

    m_ultimoPunto = actual;
}

// ── mousereleaseevent ─────────────────────────────────────────────
void Pintura::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !m_dibujando) return;
    m_dibujando = false;

    // trazo demasiado corto: descartarlo
    if (m_trazoActivo.puntos.size() < 2) {
        if (m_adminDB && m_adminDB->getDB().isOpen() && m_trazoActivo.segmentoId > 0)
            m_adminDB->borrarSegmento(m_trazoActivo.segmentoId);
        return;
    }

    // guardar en historial de memoria
    TrazoSegmento seg;
    seg.id     = m_trazoActivo.segmentoId;
    seg.color  = m_trazoActivo.color;
    seg.grosor = m_trazoActivo.grosor;
    seg.puntos = m_trazoActivo.puntos;
    m_historial.append(seg);

    // pila de deshacer (max. 10)
    if (m_trazoActivo.segmentoId > 0) {
        m_pilaDeshacer.append(m_trazoActivo.segmentoId);
        if (m_pilaDeshacer.size() > MAX_UNDO)
            m_pilaDeshacer.removeFirst();
    }

    emit statusMessage(
        QString("Trazo guardado | Color: %1 | Grosor: %2 px")
            .arg(m_color.name()).arg(m_grosor)
        );
}

// ── wheelevent — ajusta grosor ────────────────────────────────────
void Pintura::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y() > 0 ? 1 : -1;
    setGrosor(m_grosor + delta);
    emit statusMessage(QString("Grosor: %1 px").arg(m_grosor));
    event->accept();
}

// ── keypressevent ─────────────────────────────────────────────────
void Pintura::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)) {
        deshacerUltimo();
        return;
    }

    switch (event->key()) {
    case Qt::Key_R: setColor(Qt::red);   emit statusMessage("Color: Rojo");   break;
    case Qt::Key_G: setColor(Qt::green); emit statusMessage("Color: Verde");  break;
    case Qt::Key_B: setColor(Qt::blue);  emit statusMessage("Color: Azul");   break;
    case Qt::Key_K: setColor(Qt::black); emit statusMessage("Color: Negro");  break;
    case Qt::Key_W: setColor(Qt::white); emit statusMessage("Color: Blanco (borrador)"); break;
    case Qt::Key_Escape: borrarTodo(); break;
    default: QWidget::keyPressEvent(event);
    }
}