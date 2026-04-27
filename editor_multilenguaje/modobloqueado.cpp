#include "modobloqueado.h"
#include "gestorlog.h"
#include "gestorconfig.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QApplication>
#include <QMessageBox>

ModoBloqueado::ModoBloqueado(QWidget *parent)
    : Pantalla(parent),
    m_segundosRestantes(30)
{
    inicializarUI();
    conectarEventos();
    cargarDatos();
    registrarEvento("Pantalla ModoBloqueado inicializada.");
}

ModoBloqueado::~ModoBloqueado()
{
    registrarEvento("Pantalla ModoBloqueado destruida.");
}

void ModoBloqueado::setTiempoBloqueo(int segundos)
{
    m_segundosRestantes = segundos;
    m_lblContador->setText(QString("Reintentá en %1 segundos...").arg(m_segundosRestantes));
}

// ============================================================
// Implementación de interfaz Pantalla
// ============================================================

void ModoBloqueado::inicializarUI()
{
    setWindowTitle("Editor Multilenguaje - Acceso Bloqueado");
    setFixedSize(440, 420);
    setStyleSheet(R"(
        QWidget {
            background-color: #1a0000;
        }
        QLabel {
            color: #ffcccc;
            font-family: 'Segoe UI', sans-serif;
        }
        QLabel#icono {
            font-size: 64px;
        }
        QLabel#mensaje {
            font-size: 18px;
            font-weight: bold;
            color: #ff5555;
        }
        QLabel#contador {
            font-size: 15px;
            color: #ffaa00;
        }
    )");

    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignCenter);
    m_layout->setSpacing(20);
    m_layout->setContentsMargins(40, 60, 40, 60);

    m_lblIcono = new QLabel("🔒", this);
    m_lblIcono->setObjectName("icono");
    m_lblIcono->setAlignment(Qt::AlignCenter);

    m_lblMensaje = new QLabel("Acceso bloqueado temporalmente", this);
    m_lblMensaje->setObjectName("mensaje");
    m_lblMensaje->setAlignment(Qt::AlignCenter);
    m_lblMensaje->setWordWrap(true);

    QLabel *lblInfo = new QLabel("Se superó el número máximo de intentos fallidos.\nEsperá para volver a intentarlo.", this);
    lblInfo->setAlignment(Qt::AlignCenter);
    lblInfo->setWordWrap(true);
    lblInfo->setStyleSheet("color: #cc8888; font-size: 13px;");

    m_lblContador = new QLabel("", this);
    m_lblContador->setObjectName("contador");
    m_lblContador->setAlignment(Qt::AlignCenter);

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    m_layout->addWidget(m_lblIcono);
    m_layout->addWidget(m_lblMensaje);
    m_layout->addWidget(lblInfo);
    m_layout->addWidget(m_lblContador);
    setLayout(m_layout);
}

void ModoBloqueado::conectarEventos()
{
    connect(m_timer, &QTimer::timeout, this, &ModoBloqueado::tickContador);
}

void ModoBloqueado::cargarDatos()
{
    m_segundosRestantes = GestorConfig::instancia().tiempoBloqueoSegundos();
    m_lblContador->setText(QString("Reintentá en %1 segundos...").arg(m_segundosRestantes));
    m_timer->start();
}

bool ModoBloqueado::validarEstado()
{
    return m_segundosRestantes <= 0;
}

void ModoBloqueado::registrarEvento(const QString &descripcion)
{
    GestorLog::instancia().registrar(nombrePantalla(), descripcion);
}

QString ModoBloqueado::nombrePantalla() const
{
    return "ModoBloqueado";
}

// ============================================================
// Slot
// ============================================================

void ModoBloqueado::tickContador()
{
    m_segundosRestantes--;
    if (m_segundosRestantes <= 0) {
        m_timer->stop();
        registrarEvento("Tiempo de bloqueo agotado. Redirigiendo a Login.");
        emit solicitudCambioPantalla("Login");
    } else {
        m_lblContador->setText(QString("Reintentá en %1 segundos...").arg(m_segundosRestantes));
    }
}

// ============================================================
// Redefinición de eventos
// ============================================================

void ModoBloqueado::keyPressEvent(QKeyEvent *event)
{
    // En modo bloqueado, ignoramos todas las teclas
    registrarEvento(QString("keyPressEvent ignorado en ModoBloqueado: tecla=%1").arg(event->key()));
    // No propagamos para bloquear interacción
}

void ModoBloqueado::mousePressEvent(QMouseEvent *event)
{
    registrarEvento(QString("mousePressEvent en ModoBloqueado: pos=(%1,%2)")
                        .arg(event->pos().x()).arg(event->pos().y()));
}

void ModoBloqueado::resizeEvent(QResizeEvent *event)
{
    registrarEvento(QString("resizeEvent en ModoBloqueado: %1x%2")
                        .arg(event->size().width()).arg(event->size().height()));
    Pantalla::resizeEvent(event);
}

void ModoBloqueado::closeEvent(QCloseEvent *event)
{
    registrarEvento("closeEvent en ModoBloqueado: cierre solicitado.");
    QMessageBox::StandardButton resp = QMessageBox::warning(
        this,
        "Salir",
        "La cuenta está bloqueada. ¿Cerrás la aplicación de todas formas?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (resp == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ModoBloqueado::focusInEvent(QFocusEvent *event)
{
    registrarEvento("focusInEvent: ModoBloqueado recibió el foco.");
    Pantalla::focusInEvent(event);
}

void ModoBloqueado::focusOutEvent(QFocusEvent *event)
{
    registrarEvento("focusOutEvent: ModoBloqueado perdió el foco.");
    Pantalla::focusOutEvent(event);
}
