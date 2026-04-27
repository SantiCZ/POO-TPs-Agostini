#include "login.h"
#include "gestorlog.h"
#include "gestorconfig.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QApplication>
#include <QMessageBox>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

Login::Login(QWidget *parent)
    : Pantalla(parent),
    m_intentosFallidos(0),
    m_segundosRestantes(0),
    m_estaBloqueado(false)
{
    inicializarUI();
    conectarEventos();
    cargarDatos();
    registrarEvento("Pantalla de Login inicializada.");
}

Login::~Login()
{
    registrarEvento("Pantalla de Login destruida.");
}

// ============================================================
// Implementación de interfaz Pantalla
// ============================================================

void Login::inicializarUI()
{
    setWindowTitle("Editor Multilenguaje - Login");
    setFixedSize(440, 520);
    setStyleSheet(R"(
        QWidget {
            background-color: #0f0f1a;
            font-family: 'Segoe UI', sans-serif;
        }
        QLabel#titulo {
            color: #e0e0ff;
            font-size: 24px;
            font-weight: bold;
        }
        QLabel#subtitulo {
            color: #7070aa;
            font-size: 13px;
        }
        QLabel {
            color: #a0a0cc;
            font-size: 13px;
        }
        QLineEdit {
            background: #1a1a2e;
            border: 1px solid #3030aa;
            border-radius: 8px;
            color: #e0e0ff;
            padding: 10px 14px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #6060ff;
        }
        QPushButton {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #3030cc, stop:1 #6030ff);
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px;
            font-size: 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #4040dd, stop:1 #7040ff);
        }
        QPushButton:disabled {
            background: #2a2a3a;
            color: #505070;
        }
    )");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(50, 50, 50, 50);
    m_layout->setSpacing(16);

    // Título
    m_lblTitulo = new QLabel("🖥  Editor Multilenguaje", this);
    m_lblTitulo->setObjectName("titulo");
    m_lblTitulo->setAlignment(Qt::AlignCenter);

    QLabel *lblSub = new QLabel("Iniciá sesión para continuar", this);
    lblSub->setObjectName("subtitulo");
    lblSub->setAlignment(Qt::AlignCenter);

    // Separador
    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #2a2a4a;");

    m_lblUsuario = new QLabel("Usuario", this);
    m_txtUsuario = new QLineEdit(this);
    m_txtUsuario->setPlaceholderText("Ingresá tu usuario");

    m_lblClave = new QLabel("Contraseña", this);
    m_txtClave = new QLineEdit(this);
    m_txtClave->setEchoMode(QLineEdit::Password);
    m_txtClave->setPlaceholderText("Ingresá tu contraseña");

    m_btnLogin = new QPushButton("Iniciar sesión", this);

    m_lblMensaje = new QLabel("", this);
    m_lblMensaje->setAlignment(Qt::AlignCenter);
    m_lblMensaje->setStyleSheet("color: #ff4444; font-size: 12px;");

    m_lblContador = new QLabel("", this);
    m_lblContador->setAlignment(Qt::AlignCenter);
    m_lblContador->setStyleSheet("color: #ff8800; font-size: 13px; font-weight: bold;");

    m_timerBloqueo = new QTimer(this);
    m_timerBloqueo->setInterval(1000);

    m_layout->addWidget(m_lblTitulo);
    m_layout->addWidget(lblSub);
    m_layout->addWidget(sep);
    m_layout->addSpacing(8);
    m_layout->addWidget(m_lblUsuario);
    m_layout->addWidget(m_txtUsuario);
    m_layout->addWidget(m_lblClave);
    m_layout->addWidget(m_txtClave);
    m_layout->addSpacing(8);
    m_layout->addWidget(m_btnLogin);
    m_layout->addWidget(m_lblMensaje);
    m_layout->addWidget(m_lblContador);

    setLayout(m_layout);
}

void Login::conectarEventos()
{
    // Signal/Slot: botón login
    connect(m_btnLogin, &QPushButton::clicked, this, &Login::intentarLogin);
    // Signal/Slot: Enter en el campo contraseña dispara login
    connect(m_txtClave, &QLineEdit::returnPressed, this, &Login::intentarLogin);
    // Signal/Slot: timer de bloqueo
    connect(m_timerBloqueo, &QTimer::timeout, this, &Login::actualizarContadorBloqueo);
}

void Login::cargarDatos()
{
    // Precargar usuario por defecto si está configurado
    m_txtUsuario->setText(GestorConfig::instancia().usuario());
    m_txtUsuario->selectAll();
}

bool Login::validarEstado()
{
    return !m_estaBloqueado &&
           !m_txtUsuario->text().isEmpty() &&
           !m_txtClave->text().isEmpty();
}

void Login::registrarEvento(const QString &descripcion)
{
    GestorLog::instancia().registrar(nombrePantalla(), descripcion);
}

QString Login::nombrePantalla() const
{
    return "Login";
}

// ============================================================
// Slots
// ============================================================

void Login::intentarLogin()
{
    if (m_estaBloqueado) return;

    if (!validarEstado()) {
        m_lblMensaje->setText("Completá ambos campos para continuar.");
        registrarEvento("Intento de login con campos vacíos.");
        return;
    }

    GestorConfig &cfg = GestorConfig::instancia();
    QString user = m_txtUsuario->text().trimmed();
    QString pass = m_txtClave->text();

    if (user == cfg.usuario() && pass == cfg.clave()) {
        registrarEvento("Login exitoso para usuario: " + user);
        m_lblMensaje->setStyleSheet("color: #44ff88; font-size: 12px;");
        m_lblMensaje->setText("✔ Acceso concedido. Abriendo editor...");
        emit solicitudCambioPantalla("EditorPrincipal");
    } else {
        m_intentosFallidos++;
        registrarEvento(QString("Login fallido. Intento %1 de %2.")
                            .arg(m_intentosFallidos)
                            .arg(cfg.intentosMax()));

        if (m_intentosFallidos >= cfg.intentosMax()) {
            // Bloqueo temporal
            m_estaBloqueado = true;
            m_segundosRestantes = cfg.tiempoBloqueoSegundos();
            aplicarEstiloBloqueado(true);
            m_timerBloqueo->start();
            emit solicitudCambioPantalla("ModoBloqueado");
            registrarEvento("Cuenta bloqueada por exceso de intentos fallidos.");
        } else {
            int restantes = cfg.intentosMax() - m_intentosFallidos;
            m_lblMensaje->setText(QString("Usuario o contraseña incorrectos. %1 intento(s) restante(s).").arg(restantes));
        }
        m_txtClave->clear();
        m_txtClave->setFocus();
    }
}

void Login::actualizarContadorBloqueo()
{
    m_segundosRestantes--;
    if (m_segundosRestantes <= 0) {
        m_timerBloqueo->stop();
        m_estaBloqueado = false;
        m_intentosFallidos = 0;
        aplicarEstiloBloqueado(false);
        m_lblContador->setText("");
        m_lblMensaje->setText("Podés intentar nuevamente.");
        registrarEvento("Bloqueo temporal finalizado.");
    } else {
        m_lblContador->setText(QString("🔒 Bloqueado. Reintentá en %1 seg.").arg(m_segundosRestantes));
    }
}

void Login::aplicarEstiloBloqueado(bool bloqueado)
{
    m_btnLogin->setEnabled(!bloqueado);
    m_txtUsuario->setEnabled(!bloqueado);
    m_txtClave->setEnabled(!bloqueado);
    if (bloqueado) {
        m_lblMensaje->setStyleSheet("color: #ff4444; font-size: 12px;");
        m_lblMensaje->setText("🔒 Acceso bloqueado temporalmente.");
    } else {
        m_lblMensaje->setStyleSheet("color: #44ff88; font-size: 12px;");
    }
}

// ============================================================
// Redefinición de eventos
// ============================================================

void Login::keyPressEvent(QKeyEvent *event)
{
    registrarEvento(QString("keyPressEvent: tecla=%1").arg(event->key()));

    // Escape limpia campos
    if (event->key() == Qt::Key_Escape) {
        m_txtUsuario->clear();
        m_txtClave->clear();
        m_txtUsuario->setFocus();
        registrarEvento("Escape: campos limpiados.");
    }
    Pantalla::keyPressEvent(event);
}

void Login::mousePressEvent(QMouseEvent *event)
{
    registrarEvento(QString("mousePressEvent en Login: pos=(%1,%2)")
                        .arg(event->pos().x()).arg(event->pos().y()));
    Pantalla::mousePressEvent(event);
}

void Login::resizeEvent(QResizeEvent *event)
{
    // Login tiene tamaño fijo, pero registramos igual
    registrarEvento(QString("resizeEvent en Login: %1x%2")
                        .arg(event->size().width()).arg(event->size().height()));
    Pantalla::resizeEvent(event);
}

void Login::closeEvent(QCloseEvent *event)
{
    registrarEvento("closeEvent: cierre de la aplicación desde Login.");
    QMessageBox::StandardButton resp = QMessageBox::question(
        this,
        "Salir",
        "¿Querés cerrar la aplicación?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (resp == QMessageBox::Yes) {
        registrarEvento("Usuario confirmó cierre desde Login.");
        event->accept();
    } else {
        event->ignore();
    }
}

void Login::focusInEvent(QFocusEvent *event)
{
    registrarEvento("focusInEvent: Login recibió el foco.");
    Pantalla::focusInEvent(event);
}

void Login::focusOutEvent(QFocusEvent *event)
{
    registrarEvento("focusOutEvent: Login perdió el foco.");
    Pantalla::focusOutEvent(event);
}
