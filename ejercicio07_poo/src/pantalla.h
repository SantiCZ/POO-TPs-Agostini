#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>
#include <QString>

/**
 * Pantalla - Clase base abstracta con interfaz común para todas las pantallas.
 * Login y Ventana heredan de esta clase.
 */
class Pantalla : public QWidget
{
    Q_OBJECT

public:
    explicit Pantalla(QWidget* parent = nullptr);
    virtual ~Pantalla() = default;

    // Interfaz común — métodos puros que cada pantalla debe implementar
    virtual void inicializar()       = 0;
    virtual void actualizarUI()      = 0;
    virtual QString nombrePantalla() const = 0;

signals:
    void pantallaLista();
    void errorOcurrido(const QString& mensaje);

protected:
    // Método helper compartido para mostrar mensajes de error amigables
    void mostrarError(const QString& titulo, const QString& mensaje);
    void mostrarInfo (const QString& titulo, const QString& mensaje);
};

#endif // PANTALLA_H
