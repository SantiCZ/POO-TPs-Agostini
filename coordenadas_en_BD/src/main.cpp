#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include "admindb.h"
#include "logger.h"
#include "loginwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("coordenadas_en_BD");

    // ── 1. logger de archivo (igual que el apunte) ────────────────
    if (!Logger::instance().init("accesos.log"))
        qWarning() << "No se pudo abrir accesos.log";

    // ── 2. conexion a la bd (usando admindb como en el apunte) ────
    qDebug() << "Directorio de trabajo:" << QDir::currentPath();

    AdminDB adminDB;
    if (adminDB.conectar("coordenadas_en_BD.db")) {
        qDebug() << "Conexion exitosa";
    } else {
        QMessageBox::critical(nullptr, "Error de base de datos",
                              "No se pudo abrir coordenadas_en_BD.db");
        return 1;
    }

    // ── 3. login ──────────────────────────────────────────────────
    LoginWindow login(&adminDB);
    if (login.exec() != QDialog::Accepted)
        return 0;

    // ── 4. ventana principal de dibujo ────────────────────────────
    MainWindow ventana(&adminDB, login.usuarioLogueado());
    ventana.show();

    return app.exec();
}