#include "editorprincipal.h"
#include "gestorlog.h"
#include "gestorconfig.h"
#include "validadorcpp.h"
#include "validadorpython.h"
#include "validadorjava.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextBlock>
#include <QScrollBar>
#include <QApplication>
#include <QScreen>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColor>

// ============================================================
// EditorDeTexto: implementación del editor con eventos custom
// ============================================================

EditorDeTexto::EditorDeTexto(QWidget *parent)
    : QPlainTextEdit(parent),
    m_ultimaLinea(-1)
{
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTabStopDistance(28.0); // 4 espacios aprox
}

void EditorDeTexto::keyPressEvent(QKeyEvent *event)
{
    // Detectar si se abandona una línea (Enter o Tab)
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        int lineaActual = textCursor().blockNumber();
        QString contenido = document()->findBlockByNumber(lineaActual).text();
        emit lineaAbandonada(lineaActual, contenido);
    }
    emit teclaPresionada(event->key(), event->modifiers());
    QPlainTextEdit::keyPressEvent(event);
}

void EditorDeTexto::mousePressEvent(QMouseEvent *event)
{
    // Al clickear en otra posición, validar la línea que se estaba editando
    int lineaActual = textCursor().blockNumber();
    if (lineaActual != m_ultimaLinea) {
        if (m_ultimaLinea >= 0) {
            QString contenido = document()->findBlockByNumber(m_ultimaLinea).text();
            emit lineaAbandonada(m_ultimaLinea, contenido);
        }
        m_ultimaLinea = lineaActual;
    }
    emit mousePresionado(event->pos(), event->button());
    QPlainTextEdit::mousePressEvent(event);
}

void EditorDeTexto::focusInEvent(QFocusEvent *event)
{
    m_ultimaLinea = textCursor().blockNumber();
    QPlainTextEdit::focusInEvent(event);
}

void EditorDeTexto::focusOutEvent(QFocusEvent *event)
{
    // Al perder foco, validar la línea actual
    int lineaActual = textCursor().blockNumber();
    QString contenido = document()->findBlockByNumber(lineaActual).text();
    emit lineaAbandonada(lineaActual, contenido);
    QPlainTextEdit::focusOutEvent(event);
}

// ============================================================
// EditorPrincipal: implementación
// ============================================================

EditorPrincipal::EditorPrincipal(QWidget *parent)
    : Pantalla(parent),
    m_validador(nullptr),
    m_lenguajeActual("C++"),
    m_modificado(false)
{
    inicializarUI();
    conectarEventos();
    cargarDatos();
    registrarEvento("EditorPrincipal inicializado.");
}

EditorPrincipal::~EditorPrincipal()
{
    delete m_validador;
    registrarEvento("EditorPrincipal destruido.");
}

// ============================================================
// Interfaz Pantalla
// ============================================================

void EditorPrincipal::inicializarUI()
{
    setWindowTitle("Editor Multilenguaje");
    showFullScreen();

    setStyleSheet(R"(
        QWidget {
            background-color: #0d0d1a;
            font-family: 'Segoe UI', sans-serif;
        }
        QToolBar, QFrame#toolbar {
            background: #12122a;
            border-bottom: 1px solid #2a2a4a;
        }
        QComboBox {
            background: #1a1a2e;
            color: #c0c0ff;
            border: 1px solid #3030aa;
            border-radius: 6px;
            padding: 4px 10px;
            font-size: 13px;
            min-width: 120px;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background: #1a1a2e;
            color: #c0c0ff;
            selection-background-color: #3030aa;
        }
        QPushButton {
            background: #1e1e3a;
            color: #9090cc;
            border: 1px solid #3030aa;
            border-radius: 6px;
            padding: 6px 14px;
            font-size: 13px;
        }
        QPushButton:hover {
            background: #2a2a5a;
            color: #e0e0ff;
        }
        QPushButton#btnExportar {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #1a6a1a, stop:1 #2a9a2a);
            color: #ccffcc;
            border: none;
        }
        QPushButton#btnExportar:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #2a8a2a, stop:1 #3aaa3a);
        }
        QLabel#lblDiagnostico {
            color: #ff6666;
            font-size: 12px;
            padding: 4px 12px;
            background: #2a0000;
            border-top: 1px solid #5a1a1a;
        }
        QLabel#lblEstado {
            color: #6060aa;
            font-size: 12px;
            padding: 4px 12px;
            background: #0d0d1a;
            border-top: 1px solid #2a2a4a;
        }
        QSplitter::handle {
            background: #2a2a4a;
            width: 2px;
        }
    )");

    // ---- Toolbar principal ----
    QFrame *toolbar = new QFrame(this);
    toolbar->setObjectName("toolbar");
    toolbar->setFixedHeight(54);
    m_layoutToolbar = new QHBoxLayout(toolbar);
    m_layoutToolbar->setContentsMargins(16, 6, 16, 6);
    m_layoutToolbar->setSpacing(12);

    QLabel *appLabel = new QLabel("🖥  Editor Multilenguaje", toolbar);
    appLabel->setStyleSheet("color:#a0a0ff; font-size:15px; font-weight:bold;");

    m_lblLenguaje = new QLabel("Lenguaje:", toolbar);
    m_lblLenguaje->setStyleSheet("color:#7070aa; font-size:13px;");

    m_cmbLenguaje = new QComboBox(toolbar);
    m_cmbLenguaje->addItems({"C++", "Python", "Java"});

    m_btnNuevo = new QPushButton("⊕ Nuevo", toolbar);
    m_btnLimpiarErrores = new QPushButton("✖ Limpiar errores", toolbar);

    m_btnExportar = new QPushButton("⬇ Exportar JPG", toolbar);
    m_btnExportar->setObjectName("btnExportar");

    m_layoutToolbar->addWidget(appLabel);
    m_layoutToolbar->addStretch();
    m_layoutToolbar->addWidget(m_lblLenguaje);
    m_layoutToolbar->addWidget(m_cmbLenguaje);
    m_layoutToolbar->addWidget(m_btnNuevo);
    m_layoutToolbar->addWidget(m_btnLimpiarErrores);
    m_layoutToolbar->addWidget(m_btnExportar);

    // ---- Editor de texto ----
    m_editor = new EditorDeTexto(this);
    m_editor->setStyleSheet(R"(
        QPlainTextEdit {
            background: #0d0d1a;
            color: #d0d0ff;
            border: none;
            font-family: 'Courier New', monospace;
            font-size: 14px;
            padding: 10px;
        }
        QScrollBar:vertical {
            background: #12122a;
            width: 10px;
            border: none;
        }
        QScrollBar::handle:vertical {
            background: #3030aa;
            border-radius: 4px;
        }
    )");
    m_editor->setPlaceholderText("// Escribí tu código aquí...");

    // ---- Panel CV ----
    m_panelCV = new PanelCurriculum(this);

    // ---- Splitter: editor | curriculum ----
    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_editor);
    m_splitter->addWidget(m_panelCV);
    m_splitter->setStretchFactor(0, 3);
    m_splitter->setStretchFactor(1, 1);

    // ---- Barra de diagnóstico ----
    m_lblDiagnostico = new QLabel("", this);
    m_lblDiagnostico->setObjectName("lblDiagnostico");
    m_lblDiagnostico->setWordWrap(true);
    m_lblDiagnostico->setFixedHeight(28);
    m_lblDiagnostico->hide();

    // ---- Barra de estado ----
    m_lblEstado = new QLabel("Listo | Línea 1, Col 1 | C++", this);
    m_lblEstado->setObjectName("lblEstado");
    m_lblEstado->setFixedHeight(26);

    // ---- Layout principal ----
    m_layoutPrincipal = new QVBoxLayout(this);
    m_layoutPrincipal->setContentsMargins(0, 0, 0, 0);
    m_layoutPrincipal->setSpacing(0);
    m_layoutPrincipal->addWidget(toolbar);
    m_layoutPrincipal->addWidget(m_splitter, 1);
    m_layoutPrincipal->addWidget(m_lblDiagnostico);
    m_layoutPrincipal->addWidget(m_lblEstado);

    setLayout(m_layoutPrincipal);
}

void EditorPrincipal::conectarEventos()
{
    // Cambio de lenguaje (Signal → Slot)
    connect(m_cmbLenguaje, &QComboBox::currentTextChanged,
            this, &EditorPrincipal::cambiarLenguaje);

    // Validación al abandonar la línea (EditorDeTexto → EditorPrincipal)
    connect(m_editor, &EditorDeTexto::lineaAbandonada,
            this, &EditorPrincipal::validarLinea);

    // Manejo de teclas del editor
    connect(m_editor, &EditorDeTexto::teclaPresionada,
            this, &EditorPrincipal::manejarTecla);

    // Manejo de mouse
    connect(m_editor, &EditorDeTexto::mousePresionado,
            this, &EditorPrincipal::manejarMouse);

    // Botón exportar
    connect(m_btnExportar, &QPushButton::clicked,
            this, &EditorPrincipal::exportarJpg);

    // Botón nuevo
    connect(m_btnNuevo, &QPushButton::clicked,
            this, &EditorPrincipal::nuevoArchivo);

    // Botón limpiar errores
    connect(m_btnLimpiarErrores, &QPushButton::clicked,
            this, &EditorPrincipal::limpiarErrores);

    // Actualizar barra de estado al mover cursor
    connect(m_editor, &QPlainTextEdit::cursorPositionChanged,
            this, &EditorPrincipal::actualizarBarraEstado);

    // Detectar cambios para marcar como modificado
    connect(m_editor->document(), &QTextDocument::contentsChanged, this, [this]() {
        m_modificado = true;
    });
}

void EditorPrincipal::cargarDatos()
{
    GestorConfig &cfg = GestorConfig::instancia();
    m_lenguajeActual = cfg.lenguajeDefecto();

    // Sincronizar combo con el lenguaje configurado
    int idx = m_cmbLenguaje->findText(m_lenguajeActual);
    if (idx >= 0) m_cmbLenguaje->setCurrentIndex(idx);

    crearValidador(m_lenguajeActual);

    // Texto de bienvenida
    QString bienvenida;
    if (m_lenguajeActual == "C++") {
        bienvenida = "// Editor Multilenguaje - C++\n// Escribí tu código a continuación\n\n#include <iostream>\nusing namespace std;\n\nint main() {\n    cout << \"Hola, mundo!\" << endl;\n    return 0;\n}";
    } else if (m_lenguajeActual == "Python") {
        bienvenida = "# Editor Multilenguaje - Python\n# Escribí tu código a continuación\n\nprint(\"Hola, mundo!\")";
    } else {
        bienvenida = "// Editor Multilenguaje - Java\n// Escribí tu código a continuación\n\npublic class Main {\n    public static void main(String[] args) {\n        System.out.println(\"Hola, mundo!\");\n    }\n}";
    }
    m_editor->setPlainText(bienvenida);
    m_modificado = false;
}

bool EditorPrincipal::validarEstado()
{
    return m_validador != nullptr && !m_editor->toPlainText().isEmpty();
}

void EditorPrincipal::registrarEvento(const QString &descripcion)
{
    GestorLog::instancia().registrar(nombrePantalla(), descripcion);
}

QString EditorPrincipal::nombrePantalla() const
{
    return "EditorPrincipal";
}

// ============================================================
// Slots propios
// ============================================================

void EditorPrincipal::cambiarLenguaje(const QString &lenguaje)
{
    m_lenguajeActual = lenguaje;
    crearValidador(lenguaje);
    limpiarErrores();
    actualizarBarraEstado();
    registrarEvento("Lenguaje cambiado a: " + lenguaje);
}

void EditorPrincipal::validarLinea(int numeroLinea, const QString &contenido)
{
    if (!m_validador) return;
    if (contenido.trimmed().isEmpty()) {
        limpiarLineaError(numeroLinea);
        return;
    }

    bool esValida = m_validador->validarLinea(contenido);
    if (!esValida) {
        QString msg = m_validador->mensajeDiagnostico(contenido);
        marcarLineaError(numeroLinea, msg);
        m_lblDiagnostico->setText(QString("Línea %1: %2").arg(numeroLinea + 1).arg(msg));
        m_lblDiagnostico->show();
        registrarEvento(QString("Error de sintaxis en línea %1: %2").arg(numeroLinea + 1).arg(msg));
    } else {
        limpiarLineaError(numeroLinea);
        m_lblDiagnostico->hide();
    }
}

void EditorPrincipal::exportarJpg()
{
    registrarEvento("Exportación a JPG solicitada.");
    GestorConfig &cfg = GestorConfig::instancia();
    QString ruta = cfg.rutaExportacion();

    QString codigo = m_editor->toPlainText();
    if (codigo.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Exportar", "El editor está vacío. Escribí algo antes de exportar.");
        return;
    }

    QString archivoGenerado = m_exportador.exportar(codigo, m_lenguajeActual, ruta);
    registrarEvento("Exportación exitosa: " + archivoGenerado);
    QMessageBox::information(this, "Exportar JPG",
                             QString("✔ Código exportado exitosamente.\n\nArchivo: %1").arg(archivoGenerado));
}

void EditorPrincipal::manejarTecla(int key, Qt::KeyboardModifiers mods)
{
    // Atajos del editor
    if (mods == Qt::ControlModifier) {
        switch (key) {
        case Qt::Key_E:
            exportarJpg();
            registrarEvento("Atajo Ctrl+E: exportar JPG.");
            break;
        case Qt::Key_N:
            nuevoArchivo();
            registrarEvento("Atajo Ctrl+N: nuevo archivo.");
            break;
        case Qt::Key_L:
            limpiarErrores();
            registrarEvento("Atajo Ctrl+L: limpiar errores.");
            break;
        default:
            break;
        }
    }
    registrarEvento(QString("keyPressEvent en editor: key=%1").arg(key));
}

void EditorPrincipal::manejarMouse(const QPoint &pos, Qt::MouseButton btn)
{
    registrarEvento(QString("mousePressEvent en editor: pos=(%1,%2) btn=%3")
                        .arg(pos.x()).arg(pos.y()).arg(static_cast<int>(btn)));
}

void EditorPrincipal::actualizarBarraEstado()
{
    QTextCursor cur = m_editor->textCursor();
    int linea = cur.blockNumber() + 1;
    int col   = cur.columnNumber() + 1;
    int totalLineas = m_editor->document()->blockCount();
    QString modificado = m_modificado ? "  ●" : "";
    m_lblEstado->setText(QString("Línea %1 / %2  |  Col %3  |  %4%5  |  Ctrl+E: Exportar  |  Ctrl+N: Nuevo  |  Ctrl+L: Limpiar errores")
                             .arg(linea).arg(totalLineas).arg(col).arg(m_lenguajeActual).arg(modificado));
}

void EditorPrincipal::nuevoArchivo()
{
    if (m_modificado) {
        QMessageBox::StandardButton resp = QMessageBox::question(
            this, "Nuevo archivo",
            "Tenés cambios sin exportar. ¿Querés continuar de todas formas?",
            QMessageBox::Yes | QMessageBox::No);
        if (resp == QMessageBox::No) return;
    }
    m_editor->clear();
    m_errores.clear();
    m_lblDiagnostico->hide();
    m_modificado = false;
    registrarEvento("Nuevo archivo creado.");
}

void EditorPrincipal::limpiarErrores()
{
    // Quitar todos los resaltados de error
    QTextDocument *doc = m_editor->document();
    QTextCursor cursor(doc);
    cursor.select(QTextCursor::Document);
    QTextCharFormat fmt;
    fmt.setBackground(QColor("#0d0d1a")); // color de fondo normal
    cursor.setCharFormat(fmt);
    m_errores.clear();
    m_lblDiagnostico->hide();
    registrarEvento("Errores de sintaxis limpiados.");
}

// ============================================================
// Métodos privados
// ============================================================

void EditorPrincipal::crearValidador(const QString &lenguaje)
{
    delete m_validador;
    m_validador = nullptr;

    // Polimorfismo: puntero a clase base, objeto de clase derivada
    if (lenguaje == "C++") {
        m_validador = new ValidadorCpp();
    } else if (lenguaje == "Python") {
        m_validador = new ValidadorPython();
    } else if (lenguaje == "Java") {
        m_validador = new ValidadorJava();
    }
}

void EditorPrincipal::marcarLineaError(int numeroLinea, const QString &mensaje)
{
    m_errores[numeroLinea] = mensaje;

    QTextDocument *doc = m_editor->document();
    QTextBlock bloque = doc->findBlockByNumber(numeroLinea);
    if (!bloque.isValid()) return;

    QTextCursor cursor(bloque);
    cursor.select(QTextCursor::LineUnderCursor);

    QTextCharFormat fmt;
    fmt.setBackground(QColor("#3a0000")); // fondo rojo oscuro
    fmt.setUnderlineColor(QColor("#ff4444"));
    fmt.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    cursor.setCharFormat(fmt);
}

void EditorPrincipal::limpiarLineaError(int numeroLinea)
{
    if (!m_errores.contains(numeroLinea)) return;
    m_errores.remove(numeroLinea);

    QTextDocument *doc = m_editor->document();
    QTextBlock bloque = doc->findBlockByNumber(numeroLinea);
    if (!bloque.isValid()) return;

    QTextCursor cursor(bloque);
    cursor.select(QTextCursor::LineUnderCursor);

    QTextCharFormat fmt;
    fmt.setBackground(QColor("#0d0d1a")); // fondo normal
    fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);
    cursor.setCharFormat(fmt);
}

// ============================================================
// Redefinición de eventos de la ventana
// ============================================================

void EditorPrincipal::keyPressEvent(QKeyEvent *event)
{
    registrarEvento(QString("keyPressEvent en EditorPrincipal: key=%1").arg(event->key()));

    // F11: toggle fullscreen
    if (event->key() == Qt::Key_F11) {
        if (isFullScreen()) showNormal();
        else showFullScreen();
        registrarEvento("Toggle fullscreen con F11.");
    }
    Pantalla::keyPressEvent(event);
}

void EditorPrincipal::mousePressEvent(QMouseEvent *event)
{
    registrarEvento(QString("mousePressEvent en EditorPrincipal: pos=(%1,%2)")
                        .arg(event->pos().x()).arg(event->pos().y()));
    Pantalla::mousePressEvent(event);
}

void EditorPrincipal::resizeEvent(QResizeEvent *event)
{
    registrarEvento(QString("resizeEvent en EditorPrincipal: %1x%2")
                        .arg(event->size().width()).arg(event->size().height()));
    Pantalla::resizeEvent(event);
}

void EditorPrincipal::closeEvent(QCloseEvent *event)
{
    registrarEvento("closeEvent: cierre del editor solicitado.");

    if (m_modificado) {
        QMessageBox::StandardButton resp = QMessageBox::question(
            this,
            "Salir del editor",
            "Tenés cambios sin exportar. ¿Exportar antes de salir?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
        if (resp == QMessageBox::Yes) {
            exportarJpg();
            event->accept();
        } else if (resp == QMessageBox::No) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        QMessageBox::StandardButton resp = QMessageBox::question(
            this, "Salir", "¿Cerrar el editor?",
            QMessageBox::Yes | QMessageBox::No);
        if (resp == QMessageBox::Yes) {
            registrarEvento("Usuario confirmó cierre del editor.");
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void EditorPrincipal::focusInEvent(QFocusEvent *event)
{
    registrarEvento("focusInEvent: EditorPrincipal recibió el foco.");
    Pantalla::focusInEvent(event);
}

void EditorPrincipal::focusOutEvent(QFocusEvent *event)
{
    registrarEvento("focusOutEvent: EditorPrincipal perdió el foco. Validando línea actual...");
    // Al perder foco la ventana, validar línea en curso
    QTextCursor cur = m_editor->textCursor();
    int lineaActual = cur.blockNumber();
    QString contenido = m_editor->document()->findBlockByNumber(lineaActual).text();
    validarLinea(lineaActual, contenido);
    Pantalla::focusOutEvent(event);
}
