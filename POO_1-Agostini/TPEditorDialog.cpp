#include "TPEditorDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFrame>
#include <QUuid>
#include <QDateTime>

TPEditorDialog::TPEditorDialog(TPRepository*       tpRepo,
                               HistorialRepository* histRepo,
                               QWidget*            parent)
    : QWidget(parent)
    , m_tpRepo(tpRepo)
    , m_histRepo(histRepo)
{
    setWindowTitle("Editor de TP");
    setMinimumSize(520, 560);
    buildUI();
}

void TPEditorDialog::buildUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(10);

    // Título de la pantalla
    m_lblTitulo = new QLabel("Nuevo TP");
    m_lblTitulo->setStyleSheet("font-size:18px; font-weight:bold;");
    root->addWidget(m_lblTitulo);

    // ── Formulario principal ─────────────────────────────────────────────────
    auto* form = new QFormLayout;
    form->setSpacing(8);
    form->setLabelAlignment(Qt::AlignRight);

    m_txtNombre = new QLineEdit;
    m_txtNombre->setPlaceholderText("Ej: Algoritmos — TP 1");
    form->addRow("Nombre *:", m_txtNombre);

    m_txtDescripcion = new QLineEdit;
    m_txtDescripcion->setPlaceholderText("Descripción breve (opcional)");
    form->addRow("Descripción:", m_txtDescripcion);

    m_cmbEstado = new QComboBox;
    for (const QString& e : TrabajoPractico::estadosList())
        m_cmbEstado->addItem(e);
    form->addRow("Estado:", m_cmbEstado);

    m_cmbPrioridad = new QComboBox;
    for (const QString& p : TrabajoPractico::prioridadesList())
        m_cmbPrioridad->addItem(p);
    m_cmbPrioridad->setCurrentIndex(1); // Media por defecto
    form->addRow("Prioridad:", m_cmbPrioridad);

    m_dateEntrega = new QDateEdit;
    m_dateEntrega->setCalendarPopup(true);
    m_dateEntrega->setDate(QDate::currentDate().addDays(7));
    m_dateEntrega->setDisplayFormat("dd/MM/yyyy");
    form->addRow("Fecha de entrega:", m_dateEntrega);

    root->addLayout(form);

    // ── Separador + notas ────────────────────────────────────────────────────
    auto* sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#ddd;");
    root->addWidget(sep);

    auto* lblNotas = new QLabel("Notas del TP");
    lblNotas->setStyleSheet("font-size:14px; font-weight:bold; color:#333;");
    root->addWidget(lblNotas);

    auto* lblNotasHint = new QLabel("Usá el botón \"Guardar notas\" para persistir cambios sin cerrar el editor.");
    lblNotasHint->setStyleSheet("color:#888; font-size:11px;");
    lblNotasHint->setWordWrap(true);
    root->addWidget(lblNotasHint);

    m_txtNotas = new QTextEdit;
    m_txtNotas->setPlaceholderText("Apuntes, links, observaciones...");
    m_txtNotas->setMinimumHeight(110);
    root->addWidget(m_txtNotas, 1);

    m_lblNotasSaved = new QLabel;
    m_lblNotasSaved->setStyleSheet("color:#27ae60; font-size:11px;");
    root->addWidget(m_lblNotasSaved);

    // ── Error ────────────────────────────────────────────────────────────────
    m_lblError = new QLabel;
    m_lblError->setStyleSheet("color:#c0392b; font-size:12px;");
    m_lblError->setWordWrap(true);
    root->addWidget(m_lblError);

    // ── Botones ──────────────────────────────────────────────────────────────
    auto* btnLay = new QHBoxLayout;
    auto* btnGuardar     = new QPushButton("Guardar TP");
    auto* btnGuardarNotas= new QPushButton("Guardar notas");
    auto* btnCancelar    = new QPushButton("Cancelar");

    btnGuardar->setStyleSheet(
        "QPushButton{background:#1565c0;color:white;border:none;"
        "border-radius:4px;padding:7px 16px;font-weight:bold;}"
        "QPushButton:hover{background:#0d47a1;}"
    );
    btnGuardarNotas->setStyleSheet(
        "QPushButton{color:#1565c0;border:1px solid #1565c0;"
        "border-radius:4px;padding:7px 14px;}"
        "QPushButton:hover{background:#e3f2fd;}"
    );

    btnLay->addWidget(btnGuardar);
    btnLay->addWidget(btnGuardarNotas);
    btnLay->addStretch();
    btnLay->addWidget(btnCancelar);
    root->addLayout(btnLay);

    connect(btnGuardar,      &QPushButton::clicked, this, &TPEditorDialog::onGuardar);
    connect(btnGuardarNotas, &QPushButton::clicked, this, &TPEditorDialog::onGuardarNotas);
    connect(btnCancelar,     &QPushButton::clicked, this, &TPEditorDialog::onCancelar);
}

void TPEditorDialog::cargarTP(const QString& usuario, const QString& id) {
    m_usuario   = usuario;
    m_editingId = id;
    m_lblError->clear();
    m_lblNotasSaved->clear();

    if (id.isEmpty()) {
        // Nuevo TP
        m_lblTitulo->setText("Nuevo TP");
        m_txtNombre->clear();
        m_txtDescripcion->clear();
        m_cmbEstado->setCurrentIndex(0);
        m_cmbPrioridad->setCurrentIndex(1);
        m_dateEntrega->setDate(QDate::currentDate().addDays(7));
        m_txtNotas->clear();
    } else {
        // Edición
        TrabajoPractico* tp = m_tpRepo->buscarPorId(id);
        if (!tp) return;
        m_lblTitulo->setText("Editar TP");
        m_txtNombre->setText(tp->nombre);
        m_txtDescripcion->setText(tp->descripcion);
        m_cmbEstado->setCurrentText(TrabajoPractico::estadoToString(tp->estado));
        m_cmbPrioridad->setCurrentText(TrabajoPractico::prioridadToString(tp->prioridad));
        if (tp->fechaEntrega.isValid())
            m_dateEntrega->setDate(tp->fechaEntrega);
        else
            m_dateEntrega->setDate(QDate::currentDate().addDays(7));
        m_txtNotas->setPlainText(tp->notas);
    }
}

void TPEditorDialog::onGuardar() {
    m_lblError->clear();
    QString nombre = m_txtNombre->text().trimmed();
    if (nombre.isEmpty()) { m_lblError->setText("El nombre es obligatorio."); return; }

    if (m_editingId.isEmpty()) {
        // Nuevo
        TrabajoPractico tp;
        tp.id           = QUuid::createUuid().toString(QUuid::WithoutBraces);
        tp.usuarioOwner = m_usuario;
        tp.nombre       = nombre;
        tp.descripcion  = m_txtDescripcion->text().trimmed();
        tp.estado       = TrabajoPractico::estadoFromString(m_cmbEstado->currentText());
        tp.prioridad    = TrabajoPractico::prioridadFromString(m_cmbPrioridad->currentText());
        tp.fechaEntrega = m_dateEntrega->date();
        tp.notas        = m_txtNotas->toPlainText();
        tp.createdAt    = QDateTime::currentDateTime();
        tp.updatedAt    = QDateTime::currentDateTime();
        m_tpRepo->agregar(tp);
        m_histRepo->registrar(m_usuario, "Crear TP",
                              QString("Se creó \"%1\"").arg(nombre));
    } else {
        // Edición
        TrabajoPractico* tp = m_tpRepo->buscarPorId(m_editingId);
        if (!tp) return;
        tp->nombre      = nombre;
        tp->descripcion = m_txtDescripcion->text().trimmed();
        tp->estado      = TrabajoPractico::estadoFromString(m_cmbEstado->currentText());
        tp->prioridad   = TrabajoPractico::prioridadFromString(m_cmbPrioridad->currentText());
        tp->fechaEntrega= m_dateEntrega->date();
        tp->notas       = m_txtNotas->toPlainText();
        tp->updatedAt   = QDateTime::currentDateTime();
        m_tpRepo->actualizar(*tp);
        m_histRepo->registrar(m_usuario, "Editar TP",
                              QString("Se editó \"%1\"").arg(nombre));
    }
    emit guardadoExitoso();
}

void TPEditorDialog::onGuardarNotas() {
    if (m_editingId.isEmpty()) {
        m_lblNotasSaved->setStyleSheet("color:#c0392b; font-size:11px;");
        m_lblNotasSaved->setText("Primero guardá el TP completo.");
        return;
    }
    TrabajoPractico* tp = m_tpRepo->buscarPorId(m_editingId);
    if (!tp) return;
    tp->notas     = m_txtNotas->toPlainText();
    tp->updatedAt = QDateTime::currentDateTime();
    m_tpRepo->actualizar(*tp);
    m_histRepo->registrar(m_usuario, "Guardar notas",
                          QString("Notas de \"%1\" actualizadas").arg(tp->nombre));
    m_lblNotasSaved->setStyleSheet("color:#27ae60; font-size:11px;");
    m_lblNotasSaved->setText("Notas guardadas — " +
                             QTime::currentTime().toString("HH:mm:ss"));
}

void TPEditorDialog::onCancelar() { emit cancelado(); }
