#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>

/**
 * NetworkManager - Wrapper sobre QNetworkAccessManager.
 * Maneja proxy, detección de conectividad y errores de red.
 * Usa signals/slots tal como se vio en clase.
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject* parent = nullptr);

    void configurarProxy(const QString& host, int port,
                         const QString& user = "", const QString& pass = "");

    QNetworkReply* get(const QUrl& url);

    bool hayConexion() const;

signals:
    void sinConexion();

private:
    QNetworkAccessManager* m_nam;
    bool m_conexionDisponible;

private slots:
    void onReplyFinished(QNetworkReply* reply);
};

#endif // NETWORKMANAGER_H
