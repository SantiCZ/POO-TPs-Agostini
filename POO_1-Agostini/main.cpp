#include <QApplication>
#include <QStackedWidget>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

#include "UsuarioRepository.h"
#include "TPRepository.h"
#include "HistorialRepository.h"
#include "SessionManager.h"
#include "LoginWidget.h"
#include "MainWidget.h"
#include "TPEditorDialog.h"
#include "HistorialWidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("PlanificadorTP");
    app.setOrganizationName("FaMAF");

    // ── Rutas de datos ────────────────────────────────────────────────────────
    // Los archivos se guardan junto al ejecutable, en una subcarpeta "data/"
    QString dataDir = QDir::currentPath() + "/data";
    QDir().mkpath(dataDir);

    // ── Repositorios y servicios ──────────────────────────────────────────────
    UsuarioRepository  userRepo  (dataDir + "/usuarios.json");
    TPRepository       tpRepo    (dataDir + "/trabajos.json");
    HistorialRepository histRepo (dataDir + "/historial.json");
    SessionManager     session   (dataDir + "/session.json");

    userRepo.load();
    tpRepo.load();
    histRepo.load();

    // ── Pantallas ─────────────────────────────────────────────────────────────
    // Usamos QStackedWidget como contenedor principal (QWidget puro, no QMainWindow)
    QStackedWidget stack;
    stack.setWindowTitle("Planificador de Trabajos Prácticos");
    stack.setMinimumSize(720, 540);

    auto* loginWidget    = new LoginWidget    (&userRepo,          &stack);
    auto* mainWidget     = new MainWidget     (&tpRepo, &histRepo, &session, &stack);
    auto* editorWidget   = new TPEditorDialog (&tpRepo, &histRepo, &stack);
    auto* historialWidget= new HistorialWidget(&histRepo,          &stack);

    stack.addWidget(loginWidget);     // índice 0
    stack.addWidget(mainWidget);      // índice 1
    stack.addWidget(editorWidget);    // índice 2
    stack.addWidget(historialWidget); // índice 3

    // ── Lógica de navegación ──────────────────────────────────────────────────

    // Login exitoso
    QObject::connect(loginWidget, &LoginWidget::loginExitoso,
                     [&](const QString& username) {
        session.startSession(username);
        histRepo.registrar(username, "Login", "Sesión iniciada");
        mainWidget->setUsuario(username);
        stack.setCurrentIndex(1);
        stack.setWindowTitle("Planificador de TPs — " + username);
    });

    // Tick de sesión → actualiza contador en MainWidget
    QObject::connect(&session, &SessionManager::tickSecond,
                     mainWidget, &MainWidget::onSessionTick);

    // Sesión expirada
    QObject::connect(&session, &SessionManager::sessionExpired,
                     [&]() {
        QMessageBox::information(&stack, "Sesión expirada",
            "Tu sesión expiró por inactividad (5 minutos).\n"
            "Volvé a iniciar sesión.");
        stack.setCurrentIndex(0);
        stack.setWindowTitle("Planificador de Trabajos Prácticos");
    });

    // Logout manual
    QObject::connect(mainWidget, &MainWidget::logoutRequested,
                     [&]() {
        histRepo.registrar(session.currentUser(), "Logout", "Sesión cerrada manualmente");
        session.endSession();
        stack.setCurrentIndex(0);
        stack.setWindowTitle("Planificador de Trabajos Prácticos");
    });

    // Nuevo TP
    QObject::connect(mainWidget, &MainWidget::nuevoTPRequested,
                     [&]() {
        editorWidget->cargarTP(session.currentUser());
        stack.setCurrentIndex(2);
    });

    // Editar TP
    QObject::connect(mainWidget, &MainWidget::editarTPRequested,
                     [&](const QString& id) {
        editorWidget->cargarTP(session.currentUser(), id);
        stack.setCurrentIndex(2);
    });

    // Guardado desde editor
    QObject::connect(editorWidget, &TPEditorDialog::guardadoExitoso,
                     [&]() {
        mainWidget->refrescarGrilla();
        stack.setCurrentIndex(1);
    });

    // Cancelar editor
    QObject::connect(editorWidget, &TPEditorDialog::cancelado,
                     [&]() { stack.setCurrentIndex(1); });

    // Ver historial
    QObject::connect(mainWidget, &MainWidget::historialRequested,
                     [&]() {
        historialWidget->cargar(session.currentUser());
        stack.setCurrentIndex(3);
    });

    // Cerrar historial
    QObject::connect(historialWidget, &HistorialWidget::cerrado,
                     [&]() { stack.setCurrentIndex(1); });

    // ── Restaurar sesión previa si sigue vigente ───────────────────────────────
    if (session.tryRestoreSession()) {
        QString user = session.currentUser();
        histRepo.registrar(user, "Sesión restaurada", "Sesión activa recuperada");
        mainWidget->setUsuario(user);
        stack.setWindowTitle("Planificador de TPs — " + user);
        stack.setCurrentIndex(1);
    } else {
        stack.setCurrentIndex(0);
    }

    stack.show();
    return app.exec();
}
