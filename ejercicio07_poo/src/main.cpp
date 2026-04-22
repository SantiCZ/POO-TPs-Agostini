#include <QApplication>
#include <QDir>
#include "login.h"
#include "ventana.h"
#include "configmanager.h"
#include "networkmanager.h"
#include "logger.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Ejercicio07");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("UTN-FRC");

    // ── Inicializar logger ──────────────────────────────────────────────
    QString rutaLog = QDir::currentPath() + "/logs/app.log";
    Logger::instancia().inicializar(rutaLog);
    Logger::instancia().info("Aplicación iniciada");

    // ── Cargar configuración ────────────────────────────────────────────
    QString rutaConfig = QDir::currentPath() + "/config/config.ini";
    if (!ConfigManager::instancia().cargar(rutaConfig)) {
        Logger::instancia().advertencia("Usando configuración por defecto (config.ini no encontrado)");
    }

    // ── NetworkManager compartido ───────────────────────────────────────
    NetworkManager nm;
    ConfigManager& cfg = ConfigManager::instancia();
    if (cfg.usaProxy()) {
        nm.configurarProxy(cfg.proxyHost(), cfg.proxyPort(),
                           cfg.proxyUser(), cfg.proxyPass());
    }

    // ── Ventana principal (se crea pero no se muestra aún) ──────────────
    Ventana* ventana = new Ventana(&nm);
    ventana->inicializar();

    // ── Login ───────────────────────────────────────────────────────────
    Login* login = new Login(&nm);
    login->inicializar();

    // Conexión: login exitoso → empezar descarga imagen y esperar
    QObject::connect(login, &Login::loginExitoso, [&]() {
        Logger::instancia().info("Main: Login exitoso, iniciando descarga de imagen");
        login->hide();

        // Mostrar ventana principal SOLO cuando la imagen está lista
        QObject::connect(ventana, &Ventana::imagenLista, [ventana]() {
            Logger::instancia().info("Main: Imagen lista, mostrando ventana principal");
            ventana->showMaximized();
        });

        ventana->descargarImagenFondo();
    });

    login->show();

    Logger::instancia().info("Main: Mostrando Login");
    int resultado = app.exec();

    Logger::instancia().info(QString("Aplicación terminada con código: %1").arg(resultado));
    delete ventana;
    delete login;
    return resultado;
}
