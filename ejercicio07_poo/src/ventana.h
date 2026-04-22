#ifndef VENTANA_H
#define VENTANA_H

#include "pantalla.h"
#include "networkmanager.h"
#include <QLabel>
#include <QScrollArea>
#include <QNetworkReply>
#include <QProgressBar>
#include <QFrame>

/**
 * Ventana - Pantalla principal en fullscreen.
 * Hereda de Pantalla (implementa la interfaz común).
 * - Descarga imagen de fondo con cache local
 * - Muestra currículum estilo LinkedIn
 * - Se abre SOLO cuando la imagen terminó de descargarse
 */
class Ventana : public Pantalla
{
    Q_OBJECT

public:
    explicit Ventana(NetworkManager* nm, QWidget* parent = nullptr);
    ~Ventana() override = default;

    // Implementación de Pantalla
    void inicializar()              override;
    void actualizarUI()             override;
    QString nombrePantalla() const  override { return "Ventana Principal"; }

    void descargarImagenFondo();

signals:
    void imagenLista();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onImagenDescargada(QNetworkReply* reply);
    void onProgresoDescarga(qint64 recibidos, qint64 total);

private:
    void construirUI();
    void aplicarFondo();
    void aplicarEstilos();
    QWidget* crearSeccionCV();
    QWidget* crearTarjetaExp(const QString& titulo, const QString& empresa,
                             const QString& periodo, const QString& descripcion,
                             const QString& tecnologias);
    QWidget* crearBadge(const QString& texto);

    NetworkManager* m_nm;
    QNetworkReply*  m_replyImagen;
    QPixmap         m_pixmapFondo;
    QLabel*         m_lblFondo;
    QLabel*         m_lblCargando;
    QProgressBar*   m_progressBar;
    QScrollArea*    m_scroll;
    QWidget*        m_contenidoCV;
    bool            m_imagenCargada;
};

#endif // VENTANA_H
