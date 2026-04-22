#include "login.h"
#include "configmanager.h"
#include "logger.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QPalette>
#include <QKeyEvent>

Login::Login(NetworkManager* nm, QWidget* parent)
    : Pantalla(parent)
    , m_nm(nm)
    , m_clima(new Clima(nm, this))
    , m_timerHora(new QTimer(this))
    , m_timerBloqueo(new QTimer(this))
    , m_intentosFallidos(0)
    , m_segundosBloqueo(0)
    , m_bloqueado(false)
{
}

void Login::inicializar()
{
    construirUI();
    aplicarEstilos();

    // Timer de hora — actualiza cada segundo
    connect(m_timerHora, &QTimer::timeout,
            this,         &Login::onActualizarHora);
    m_timerHora->start(1000);
    onActualizarHora(); // primera actualización inmediata

    // Timer de bloqueo — cuenta regresiva
    connect(m_timerBloqueo, &QTimer::timeout,
            this,            &Login::onBloqueoTimer);

    // Señales del clima
    connect(m_clima, &Clima::climaActualizado,
            this,    &Login::onClimaActualizado);
    connect(m_clima, &Clima::errorClima,
            this,    &Login::onErrorClima);

    // Botón login
    connect(m_btnLogin, &QPushButton::clicked,
            this,        &Login::onBotonLogin);

    // Enter en password dispara login
    connect(m_edPassword, &QLineEdit::returnPressed,
            this,          &Login::onBotonLogin);
    connect(m_edUsuario,  &QLineEdit::returnPressed,
            this,          &Login::onBotonLogin);

    // Limpiar mensaje al escribir
    connect(m_edUsuario,  &QLineEdit::textChanged,
            this,          &Login::onTextoChanged);
    connect(m_edPassword, &QLineEdit::textChanged,
            this,          &Login::onTextoChanged);

    // Iniciar clima
    m_clima->inicializar();

    Logger::instancia().info("Login: Pantalla inicializada");
    emit pantallaLista();
}

void Login::actualizarUI()
{
    onActualizarHora();
}

void Login::construirUI()
{
    setWindowTitle("Sistema de Gestión — Login");
    setMinimumSize(900, 600);

    // Layout principal con fondo oscuro
    QHBoxLayout* layoutPrincipal = new QHBoxLayout(this);
    layoutPrincipal->setContentsMargins(0, 0, 0, 0);
    layoutPrincipal->setSpacing(0);

    // ── Panel izquierdo (branding + clima) ──────────────────────────────
    QWidget* panelIzq = new QWidget();
    panelIzq->setObjectName("panelIzquierdo");
    panelIzq->setMinimumWidth(380);
    QVBoxLayout* layIzq = new QVBoxLayout(panelIzq);
    layIzq->setContentsMargins(50, 60, 50, 50);
    layIzq->setSpacing(0);

    m_lblTitulo = new QLabel("Sistema de\nGestión");
    m_lblTitulo->setObjectName("lblTitulo");
    m_lblTitulo->setWordWrap(true);

    m_lblSubtitulo = new QLabel("Ejercicio 07 — Qt Network");
    m_lblSubtitulo->setObjectName("lblSubtitulo");

    layIzq->addWidget(m_lblTitulo);
    layIzq->addSpacing(8);
    layIzq->addWidget(m_lblSubtitulo);
    layIzq->addStretch();

    // Bloque clima
    QFrame* frameClima = new QFrame();
    frameClima->setObjectName("frameClima");
    QVBoxLayout* layClima = new QVBoxLayout(frameClima);
    layClima->setContentsMargins(20, 20, 20, 20);
    layClima->setSpacing(4);

    m_lblHora = new QLabel("--:--:--");
    m_lblHora->setObjectName("lblHora");
    m_lblFecha = new QLabel("---");
    m_lblFecha->setObjectName("lblFecha");
    m_lblClima = new QLabel("🌤  --°C  |  ---");
    m_lblClima->setObjectName("lblClima");
    m_lblOffline = new QLabel("⚠ Modo offline");
    m_lblOffline->setObjectName("lblOffline");
    m_lblOffline->setVisible(false);

    layClima->addWidget(m_lblHora);
    layClima->addWidget(m_lblFecha);
    layClima->addWidget(m_lblClima);
    layClima->addWidget(m_lblOffline);
    layIzq->addWidget(frameClima);

    layoutPrincipal->addWidget(panelIzq);

    // ── Panel derecho (formulario) ──────────────────────────────────────
    QWidget* panelDer = new QWidget();
    panelDer->setObjectName("panelDerecho");
    QVBoxLayout* layDer = new QVBoxLayout(panelDer);
    layDer->setContentsMargins(60, 0, 60, 0);
    layDer->setAlignment(Qt::AlignCenter);

    // Card del formulario
    m_card = new QFrame();
    m_card->setObjectName("card");
    m_card->setMaximumWidth(400);
    QVBoxLayout* layCard = new QVBoxLayout(m_card);
    layCard->setContentsMargins(40, 40, 40, 40);
    layCard->setSpacing(16);

    QLabel* lblFormTitulo = new QLabel("Iniciar sesión");
    lblFormTitulo->setObjectName("lblFormTitulo");

    QLabel* lblUsuario = new QLabel("Usuario");
    lblUsuario->setObjectName("lblCampo");
    m_edUsuario = new QLineEdit();
    m_edUsuario->setPlaceholderText("Ingresá tu usuario");
    m_edUsuario->setObjectName("edInput");

    QLabel* lblPassword = new QLabel("Contraseña");
    lblPassword->setObjectName("lblCampo");
    m_edPassword = new QLineEdit();
    m_edPassword->setEchoMode(QLineEdit::Password);
    m_edPassword->setPlaceholderText("Ingresá tu contraseña");
    m_edPassword->setObjectName("edInput");

    m_lblMensaje = new QLabel("");
    m_lblMensaje->setObjectName("lblMensaje");
    m_lblMensaje->setWordWrap(true);
    m_lblMensaje->setVisible(false);

    m_btnLogin = new QPushButton("Ingresar");
    m_btnLogin->setObjectName("btnLogin");
    m_btnLogin->setCursor(Qt::PointingHandCursor);

    layCard->addWidget(lblFormTitulo);
    layCard->addSpacing(8);
    layCard->addWidget(lblUsuario);
    layCard->addWidget(m_edUsuario);
    layCard->addWidget(lblPassword);
    layCard->addWidget(m_edPassword);
    layCard->addWidget(m_lblMensaje);
    layCard->addSpacing(8);
    layCard->addWidget(m_btnLogin);

    // Sombra al card
    QGraphicsDropShadowEffect* sombra = new QGraphicsDropShadowEffect();
    sombra->setBlurRadius(30);
    sombra->setOffset(0, 8);
    sombra->setColor(QColor(0, 0, 0, 60));
    m_card->setGraphicsEffect(sombra);

    layDer->addWidget(m_card, 0, Qt::AlignCenter);
    layDer->setAlignment(m_card, Qt::AlignHCenter | Qt::AlignVCenter);
    layoutPrincipal->addWidget(panelDer, 1);
}

void Login::aplicarEstilos()
{
    setStyleSheet(R"(
        Login {
            background-color: #0f172a;
        }

        #panelIzquierdo {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1e3a5f, stop:1 #0f172a);
            border-right: 1px solid #1e293b;
        }

        #lblTitulo {
            font-family: "Georgia", serif;
            font-size: 42px;
            font-weight: bold;
            color: #f8fafc;
            line-height: 1.1;
        }

        #lblSubtitulo {
            font-family: "Courier New", monospace;
            font-size: 11px;
            color: #64748b;
            letter-spacing: 2px;
            text-transform: uppercase;
        }

        #frameClima {
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(255, 255, 255, 0.08);
            border-radius: 12px;
        }

        #lblHora {
            font-family: "Courier New", monospace;
            font-size: 38px;
            font-weight: bold;
            color: #e2e8f0;
            letter-spacing: 2px;
        }

        #lblFecha {
            font-size: 13px;
            color: #94a3b8;
        }

        #lblClima {
            font-size: 15px;
            color: #7dd3fc;
            margin-top: 8px;
        }

        #lblOffline {
            font-size: 11px;
            color: #fbbf24;
            background: rgba(251, 191, 36, 0.1);
            border-radius: 4px;
            padding: 3px 8px;
        }

        #panelDerecho {
            background-color: #0f172a;
        }

        #card {
            background: #1e293b;
            border-radius: 16px;
            border: 1px solid #334155;
            min-width: 340px;
        }

        #lblFormTitulo {
            font-size: 24px;
            font-weight: bold;
            color: #f1f5f9;
            margin-bottom: 4px;
        }

        #lblCampo {
            font-size: 12px;
            color: #94a3b8;
            margin-bottom: 2px;
        }

        #edInput {
            background: #0f172a;
            border: 1.5px solid #334155;
            border-radius: 8px;
            padding: 12px 14px;
            font-size: 14px;
            color: #f1f5f9;
        }

        #edInput:focus {
            border-color: #3b82f6;
            background: #0f172a;
        }

        #edInput::placeholder {
            color: #475569;
        }

        #btnLogin {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3b82f6, stop:1 #2563eb);
            color: white;
            border: none;
            border-radius: 8px;
            padding: 13px;
            font-size: 15px;
            font-weight: bold;
        }

        #btnLogin:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #60a5fa, stop:1 #3b82f6);
        }

        #btnLogin:pressed {
            background: #1d4ed8;
        }

        #btnLogin:disabled {
            background: #334155;
            color: #64748b;
        }

        #lblMensaje {
            color: #f87171;
            font-size: 13px;
            background: rgba(248, 113, 113, 0.08);
            border: 1px solid rgba(248, 113, 113, 0.2);
            border-radius: 6px;
            padding: 8px 12px;
        }
    )");
}

void Login::onActualizarHora()
{
    QDateTime ahora = QDateTime::currentDateTime();
    m_lblHora->setText(ahora.toString("HH:mm:ss"));

    // Capitalizar día de la semana en español
    QLocale locale(QLocale::Spanish);
    QString fecha = locale.toString(ahora, "dddd d 'de' MMMM yyyy");
    fecha[0] = fecha[0].toUpper();
    m_lblFecha->setText(fecha);
}

void Login::onClimaActualizado(double temperatura, const QString& descripcion,
                                const QString& ciudad, const QString& icono, int humedad)
{
    Q_UNUSED(icono)
    Q_UNUSED(ciudad)

    // Elegir emoji según icono
    QString emoji = "🌤";
    if (descripcion.contains("lluvia", Qt::CaseInsensitive)) emoji = "🌧";
    else if (descripcion.contains("nublado", Qt::CaseInsensitive)) emoji = "☁";
    else if (descripcion.contains("despejado", Qt::CaseInsensitive) ||
             descripcion.contains("claro", Qt::CaseInsensitive)) emoji = "☀";
    else if (descripcion.contains("tormenta", Qt::CaseInsensitive)) emoji = "⛈";
    else if (descripcion.contains("niebla", Qt::CaseInsensitive)) emoji = "🌫";

    m_lblClima->setText(QString("%1  %2°C  |  %3  |  💧%4%")
                        .arg(emoji)
                        .arg(temperatura, 0, 'f', 1)
                        .arg(descripcion)
                        .arg(humedad));

    m_lblOffline->setVisible(m_clima->offline());
}

void Login::onErrorClima(const QString& mensaje)
{
    Logger::instancia().advertencia("Login: Error clima -> " + mensaje);
    m_lblClima->setText("🌡  Sin datos de clima");
    m_lblOffline->setVisible(true);
}

void Login::onTextoChanged()
{
    if (!m_bloqueado) {
        m_lblMensaje->setVisible(false);
    }
}

void Login::onBotonLogin()
{
    if (m_bloqueado) return;

    if (validarCredenciales()) {
        Logger::instancia().info("Login: Autenticación exitosa para usuario: " + m_edUsuario->text());
        m_intentosFallidos = 0;
        emit loginExitoso();
    } else {
        m_intentosFallidos++;
        Logger::instancia().advertencia(QString("Login: Intento fallido %1/%2 para usuario: %3")
                                        .arg(m_intentosFallidos)
                                        .arg(ConfigManager::instancia().maxIntentos())
                                        .arg(m_edUsuario->text()));

        int maxIntentos = ConfigManager::instancia().maxIntentos();
        int restantes   = maxIntentos - m_intentosFallidos;

        if (m_intentosFallidos >= maxIntentos) {
            activarBloqueo();
        } else {
            m_lblMensaje->setText(QString("Usuario o contraseña incorrectos.\nQuedan %1 intento(s) antes del bloqueo.")
                                  .arg(restantes));
            m_lblMensaje->setVisible(true);
        }

        m_edPassword->clear();
        m_edPassword->setFocus();
    }
}

bool Login::validarCredenciales()
{
    ConfigManager& cfg = ConfigManager::instancia();
    return (m_edUsuario->text().trimmed() == cfg.usuarioValido() &&
            m_edPassword->text()          == cfg.passwordValido());
}

void Login::activarBloqueo()
{
    m_bloqueado        = true;
    m_segundosBloqueo  = ConfigManager::instancia().bloqueoSegundos();
    m_btnLogin->setEnabled(false);
    m_edUsuario->setEnabled(false);
    m_edPassword->setEnabled(false);

    Logger::instancia().advertencia(QString("Login: Cuenta bloqueada por %1 segundos").arg(m_segundosBloqueo));

    m_lblMensaje->setText(QString("⛔ Demasiados intentos. Bloqueado por %1 segundos.")
                          .arg(m_segundosBloqueo));
    m_lblMensaje->setVisible(true);

    m_timerBloqueo->start(1000);
}

void Login::desactivarBloqueo()
{
    m_bloqueado           = false;
    m_intentosFallidos    = 0;
    m_btnLogin->setEnabled(true);
    m_edUsuario->setEnabled(true);
    m_edPassword->setEnabled(true);
    m_lblMensaje->setVisible(false);
    m_edUsuario->setFocus();
    Logger::instancia().info("Login: Bloqueo levantado");
}

void Login::onBloqueoTimer()
{
    m_segundosBloqueo--;
    m_lblMensaje->setText(QString("⛔ Demasiados intentos. Bloqueado por %1 segundos.")
                          .arg(m_segundosBloqueo));

    if (m_segundosBloqueo <= 0) {
        m_timerBloqueo->stop();
        desactivarBloqueo();
    }
}
