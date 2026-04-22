#ifndef LOGIN_H
#define LOGIN_H

#include "pantalla.h"
#include "clima.h"
#include "networkmanager.h"
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

/**
 * Login - Pantalla de autenticación.
 * Hereda de Pantalla (implementa la interfaz común).
 * - Muestra temperatura y hora local
 * - Bloqueo temporal tras N intentos fallidos
 * - Datos de clima en tiempo real
 */
class Login : public Pantalla
{
    Q_OBJECT

public:
    explicit Login(NetworkManager* nm, QWidget* parent = nullptr);
    ~Login() override = default;

    // Implementación de Pantalla
    void inicializar()              override;
    void actualizarUI()             override;
    QString nombrePantalla() const  override { return "Login"; }

signals:
    void loginExitoso();

private slots:
    void onBotonLogin();
    void onTextoChanged();
    void onActualizarHora();
    void onClimaActualizado(double temperatura, const QString& descripcion,
                            const QString& ciudad, const QString& icono, int humedad);
    void onErrorClima(const QString& mensaje);
    void onBloqueoTimer();

private:
    void construirUI();
    void aplicarEstilos();
    bool validarCredenciales();
    void activarBloqueo();
    void desactivarBloqueo();

    // Widgets
    QLabel*      m_lblTitulo;
    QLabel*      m_lblSubtitulo;
    QLabel*      m_lblHora;
    QLabel*      m_lblFecha;
    QLabel*      m_lblClima;
    QLabel*      m_lblMensaje;
    QLineEdit*   m_edUsuario;
    QLineEdit*   m_edPassword;
    QPushButton* m_btnLogin;
    QFrame*      m_card;
    QLabel*      m_lblOffline;

    // Lógica
    NetworkManager* m_nm;
    Clima*          m_clima;
    QTimer*         m_timerHora;
    QTimer*         m_timerBloqueo;

    int  m_intentosFallidos;
    int  m_segundosBloqueo;
    bool m_bloqueado;
};

#endif // LOGIN_H
