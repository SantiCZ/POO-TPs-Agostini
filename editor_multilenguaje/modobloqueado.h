#ifndef MODOBLOQUEADO_H
#define MODOBLOQUEADO_H

#include "pantalla.h"
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

// ============================================================
// ModoBloqueado: pantalla de bloqueo temporal por intentos fallidos
// ============================================================
class ModoBloqueado : public Pantalla
{
    Q_OBJECT

public:
    explicit ModoBloqueado(QWidget *parent = nullptr);
    ~ModoBloqueado() override;

    void setTiempoBloqueo(int segundos);

    // -- Implementación de la interfaz Pantalla --
    void inicializarUI()     override;
    void conectarEventos()   override;
    void cargarDatos()       override;
    bool validarEstado()     override;
    void registrarEvento(const QString &descripcion) override;
    QString nombrePantalla() const override;

protected:
    void keyPressEvent(QKeyEvent *event)       override;
    void mousePressEvent(QMouseEvent *event)   override;
    void resizeEvent(QResizeEvent *event)      override;
    void closeEvent(QCloseEvent *event)        override;
    void focusInEvent(QFocusEvent *event)      override;
    void focusOutEvent(QFocusEvent *event)     override;

private slots:
    void tickContador();

private:
    QLabel      *m_lblIcono;
    QLabel      *m_lblMensaje;
    QLabel      *m_lblContador;
    QTimer      *m_timer;
    QVBoxLayout *m_layout;
    int          m_segundosRestantes;
};

#endif // MODOBLOQUEADO_H
