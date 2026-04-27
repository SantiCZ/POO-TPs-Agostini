#include <QApplication>
#include <QStack>
#include <QPointer>

#include "pantalla.h"          // Clase base abstracta
#include "login.h"
#include "editorprincipal.h"
#include "modobloqueado.h"
#include "gestorlog.h"
#include "gestorconfig.h"

// ============================================================
// main.cpp - Punto de entrada del Editor Multilenguaje
//
// El flujo de pantallas trabaja contra punteros de la clase
// base abstracta `Pantalla`, aprovechando el polimorfismo.
//
// Flujo:
//   Login  -->  EditorPrincipal
//     |
//     +-->  ModoBloqueado --> (tiempo agotado) --> Login
// ============================================================

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Editor Multilenguaje");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Ejercicio 08");

    // ---- Cargar configuración desde archivo ----
    GestorConfig::instancia().cargar("config.ini");
    GestorLog::instancia().setArchivoLog("eventos.log");
    GestorLog::instancia().registrar("main", "Aplicación iniciada.");

    // ---- Punteros polimórficos a la clase base ----
    // El flujo trabaja SIEMPRE contra Pantalla* — nunca contra el tipo derivado.
    Pantalla *pantallaActual = nullptr;

    QString destino = "Login";
    bool continuar = true;

    while (continuar) {

        // Liberar pantalla anterior
        delete pantallaActual;
        pantallaActual = nullptr;

        // ---- Construcción polimórfica ----
        if (destino == "Login") {
            pantallaActual = new Login();     // Login IS-A Pantalla
        } else if (destino == "EditorPrincipal") {
            pantallaActual = new EditorPrincipal();  // EditorPrincipal IS-A Pantalla
        } else if (destino == "ModoBloqueado") {
            pantallaActual = new ModoBloqueado();    // ModoBloqueado IS-A Pantalla
        } else {
            break;
        }

        // ---- Llamadas polimórficas — interfaz de Pantalla ----
        // Estas llamadas son polimórficas: la versión correcta de cada
        // función virtual se resuelve en tiempo de ejecución.
        if (!pantallaActual->validarEstado()) {
            // El estado inicial puede ser inválido (p.ej. campos vacíos en Login)
            // Esto es esperado; continuamos de todas formas.
        }

        // Registrar que la pantalla fue activada
        pantallaActual->registrarEvento("Pantalla activada y mostrada.");

        // Mostrar la pantalla actual
        pantallaActual->show();

        // ---- Reaccionar a solicitud de cambio de pantalla ----
        // Conectar señal polimórfica a una lambda que captura destino y continuar
        QString siguienteDestino = "";
        bool terminado = false;

        QObject::connect(pantallaActual, &Pantalla::solicitudCambioPantalla,
                         [&](const QString &d) {
                             siguienteDestino = d;
                             app.quit();
                         });

        // Ejecutar el loop de eventos para esta pantalla
        app.exec();

        if (siguienteDestino.isEmpty()) {
            // El usuario cerró la ventana sin navegar
            continuar = false;
        } else {
            destino = siguienteDestino;
        }
    }

    GestorLog::instancia().registrar("main", "Aplicación cerrada correctamente.");
    delete pantallaActual;
    return 0;
}
