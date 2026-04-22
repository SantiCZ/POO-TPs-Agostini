#ifndef CLIMA_H
#define CLIMA_H

#include "pantalla.h"
#include "networkmanager.h"
#include <QObject>
#include <QTimer>
#include <QNetworkReply>

/**
 * Clima - Clase derivada adicional para manejo de datos climáticos.
 * Hereda de Pantalla (implementa la interfaz común).
 * Consulta directa a OpenWeatherMap sin SDK.
 * Emite señales con los datos actualizados.
 */
class Clima : public Pantalla
{
    Q_OBJECT

public:
    explicit Clima(NetworkManager* nm, QWidget* parent = nullptr);

    // Implementación de Pantalla (abstracta)
    void inicializar()              override;
    void actualizarUI()             override;
    QString nombrePantalla() const  override { return "Clima"; }

    // Datos actuales (accessors)
    double      temperatura() const  { return m_temperatura; }
    QString     descripcion() const  { return m_descripcion; }
    QString     ciudad()      const  { return m_ciudad;      }
    QString     icono()       const  { return m_icono;       }
    int         humedad()     const  { return m_humedad;     }
    bool        offline()     const  { return m_offline;     }

    void consultarClima();

signals:
    void climaActualizado(double temperatura, const QString& descripcion,
                          const QString& ciudad, const QString& icono, int humedad);
    void errorClima(const QString& mensaje);

private slots:
    void onRespuestaClima(QNetworkReply* reply);
    void onActualizacionPeriodica();
    void onSinConexion();

private:
    void parsearRespuesta(const QByteArray& datos);
    void usarDatosOffline();

    NetworkManager* m_nm;
    QTimer*         m_timer;
    QNetworkReply*  m_replyClima;

    double  m_temperatura;
    QString m_descripcion;
    QString m_ciudad;
    QString m_icono;
    int     m_humedad;
    bool    m_offline;
};

#endif // CLIMA_H
