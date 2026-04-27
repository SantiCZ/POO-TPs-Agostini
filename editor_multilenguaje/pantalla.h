#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>
#include <QString>

// ============================================================
// Clase base abstracta: Pantalla
// Define la interfaz común para todas las pantallas del sistema.
// Usa funciones virtuales puras para forzar implementación.
// ============================================================
class Pantalla : public QWidget
{
    Q_OBJECT

public:
    explicit Pantalla(QWidget *parent = nullptr);
    virtual ~Pantalla();

    // -- Interfaz obligatoria para todas las pantallas --
    virtual void inicializarUI()     = 0;
    virtual void conectarEventos()   = 0;
    virtual void cargarDatos()       = 0;
    virtual bool validarEstado()     = 0;
    virtual void registrarEvento(const QString &descripcion) = 0;

    // Identificador descriptivo de la pantalla
    virtual QString nombrePantalla() const = 0;

signals:
    // Señal genérica para que el flujo polimórfico cambie de pantalla
    void solicitudCambioPantalla(const QString &destino);
    void eventoRegistrado(const QString &mensaje);

protected:
    // Evento de resize común: cada derivada puede expandir comportamiento
    void resizeEvent(QResizeEvent *event) override;
};

#endif // PANTALLA_H
