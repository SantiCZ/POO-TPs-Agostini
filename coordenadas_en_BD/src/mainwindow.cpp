#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QApplication>

MainWindow::MainWindow(AdminDB *adminDB, const QString &usuario, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_adminDB(adminDB)
    , m_usuario(usuario)
{
    ui->setupUi(this);
    setWindowTitle(QString("coordenadas_en_BD — Usuario: %1").arg(usuario));
    resize(1024, 720);

    // el widget pintura ocupa toda el area central
    m_pintura = new Pintura(m_adminDB, this);
    setCentralWidget(m_pintura);
    m_pintura->setFocus();

    configurarEstilo();

    connect(m_pintura, &Pintura::colorCambiado,  this, &MainWindow::onColorCambiado);
    connect(m_pintura, &Pintura::grosorCambiado, this, &MainWindow::onGrosorCambiado);
    connect(m_pintura, &Pintura::statusMessage,  this, &MainWindow::onStatusMessage);

    // cargar trazos guardados de sesiones anteriores
    m_pintura->cargarDesdeDB();

    statusBar()->showMessage(
        "R=Rojo  G=Verde  B=Azul  K=Negro  W=Borrador  |  "
        "Rueda=Grosor  |  Ctrl+Z=Deshacer  |  Esc=Borrar todo", 8000
        );
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::configurarEstilo()
{
    setStyleSheet(R"(
        QMainWindow { background-color: #1e1e2e; }
        QToolBar {
            background-color: #181825; border-bottom: 1px solid #313244;
            spacing: 6px; padding: 4px 8px;
        }
        QToolButton {
            color: #cdd6f4; background: transparent;
            border: 1px solid transparent; border-radius: 5px;
            padding: 5px 10px; font-size: 12px;
        }
        QToolButton:hover  { background: #313244; border-color: #45475a; }
        QToolButton:pressed{ background: #45475a; }
        QStatusBar { background-color: #181825; color: #6c7086;
                     font-size: 11px; border-top: 1px solid #313244; }
        QMenuBar   { background-color: #181825; color: #cdd6f4; }
        QMenuBar::item:selected { background: #313244; }
        QMenu { background: #1e1e2e; color: #cdd6f4; border: 1px solid #313244; }
        QMenu::item:selected { background: #313244; }
    )");
}

// ── slots de senales del widget pintura ───────────────────────────
void MainWindow::onColorCambiado(const QColor &c)
{
    setWindowTitle(
        QString("coordenadas_en_BD — %1  |  Color: %2")
            .arg(m_usuario).arg(c.name().toUpper())
        );
}

void MainWindow::onGrosorCambiado(int g)
{
    statusBar()->showMessage(QString("Grosor: %1 px").arg(g), 2000);
}

void MainWindow::onStatusMessage(const QString &msg)
{
    statusBar()->showMessage(msg, 3000);
}

// ── acciones de la barra de herramientas ─────────────────────────
void MainWindow::on_actionColorRojo_triggered()
{ m_pintura->setColor(Qt::red);   m_pintura->setFocus(); }

void MainWindow::on_actionColorVerde_triggered()
{ m_pintura->setColor(Qt::green); m_pintura->setFocus(); }

void MainWindow::on_actionColorAzul_triggered()
{ m_pintura->setColor(Qt::blue);  m_pintura->setFocus(); }

void MainWindow::on_actionColorNegro_triggered()
{ m_pintura->setColor(Qt::black); m_pintura->setFocus(); }

void MainWindow::on_actionBorrador_triggered()
{ m_pintura->setColor(Qt::white); m_pintura->setFocus(); }

void MainWindow::on_actionDeshacer_triggered()
{
    QKeyEvent ev(QEvent::KeyPress, Qt::Key_Z, Qt::ControlModifier);
    QApplication::sendEvent(m_pintura, &ev);
    m_pintura->setFocus();
}

void MainWindow::on_actionBorrarTodo_triggered()
{
    auto r = QMessageBox::question(this, "Borrar lienzo",
                                   "¿Desea borrar todo el lienzo y los trazos guardados?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        QKeyEvent ev(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QApplication::sendEvent(m_pintura, &ev);
    }
    m_pintura->setFocus();
}

void MainWindow::on_actionAcercaDe_triggered()
{
    QMessageBox::about(this, "Acerca de — coordenadas_en_BD",
                       "<b>Ejercicio 09 — Coordenadas en base de datos</b><br><br>"
                       "Clase AdminDB (estilo catedra) sobre QSQLITE<br>"
                       "Tablas: <i>usuarios · trazos_segmentos · trazos_puntos · logs_acceso</i><br><br>"
                       "R=Rojo  G=Verde  B=Azul  K=Negro  W=Borrador<br>"
                       "Rueda=Grosor  |  Ctrl+Z=Deshacer (10)  |  Esc=Borrar todo<br><br>"
                       "<i>BD: coordenadas_en_BD.db  |  Log: accesos.log</i>"
                       );
}