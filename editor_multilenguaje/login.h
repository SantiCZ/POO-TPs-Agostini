#ifndef LOGIN_H
#define LOGIN_H

#include "pantalla.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>

// ============================================================
// Login: pantalla de autenticación
// Hereda de Pantalla e implementa todas las funciones virtuales puras.
// Gestiona bloqueo temporal tras 3 intentos fallidos.
// ============================================================
class Login : public Pantalla
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

    // -- Implementación de la interfaz Pantalla --
    void inicializarUI()     override;
    void conectarEventos()   override;
    void cargarDatos()       override;
    bool validarEstado()     override;
    void registrarEvento(const QString &descripcion) override;
    QString nombrePantalla() const override;

protected:
    // -- Redefinición de eventos del sistema --
    void keyPressEvent(QKeyEvent *event)       override;
    void mousePressEvent(QMouseEvent *event)   override;
    void resizeEvent(QResizeEvent *event)      override;
    void closeEvent(QCloseEvent *event)        override;
    void focusInEvent(QFocusEvent *event)      override;
    void focusOutEvent(QFocusEvent *event)     override;

private slots:
    void intentarLogin();
    void actualizarContadorBloqueo();

private:
    void aplicarEstiloBloqueado(bool bloqueado);

    QLabel      *m_lblTitulo;
    QLabel      *m_lblUsuario;
    QLabel      *m_lblClave;
    QLabel      *m_lblMensaje;
    QLabel      *m_lblContador;
    QLineEdit   *m_txtUsuario;
    QLineEdit   *m_txtClave;
    QPushButton *m_btnLogin;
    QTimer      *m_timerBloqueo;
    QVBoxLayout *m_layout;

    int m_intentosFallidos;
    int m_segundosRestantes;
    bool m_estaBloqueado;
};

#endif // LOGIN_H
