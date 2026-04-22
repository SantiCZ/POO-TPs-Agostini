#include "networkmanager.h"
#include "logger.h"
#include <QNetworkRequest>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , m_conexionDisponible(true)
{
    m_nam = new QNetworkAccessManager(this);

    // Connect tal como se vio en clase
    connect(m_nam, &QNetworkAccessManager::finished,
            this,  &NetworkManager::onReplyFinished);
}

void NetworkManager::configurarProxy(const QString& host, int port,
                                     const QString& user, const QString& pass)
{
    if (host.isEmpty() || port <= 0) {
        m_nam->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
        Logger::instancia().info("NetworkManager: Sin proxy configurado");
        return;
    }

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(host);
    proxy.setPort(static_cast<quint16>(port));
    if (!user.isEmpty()) proxy.setUser(user);
    if (!pass.isEmpty()) proxy.setPassword(pass);

    m_nam->setProxy(proxy);
    Logger::instancia().info(QString("NetworkManager: Proxy configurado -> %1:%2").arg(host).arg(port));
}

QNetworkReply* NetworkManager::get(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Ejercicio07-QtApp/1.0");
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                         QNetworkRequest::NoLessSafeRedirectPolicy);

    Logger::instancia().info("NetworkManager: GET -> " + url.toString());
    return m_nam->get(request);
}

bool NetworkManager::hayConexion() const
{
    return m_conexionDisponible;
}

void NetworkManager::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NetworkSessionFailedError ||
        reply->error() == QNetworkReply::HostNotFoundError       ||
        reply->error() == QNetworkReply::TimeoutError)
    {
        if (m_conexionDisponible) {
            m_conexionDisponible = false;
            Logger::instancia().advertencia("NetworkManager: Sin conexión a internet detectada");
            emit sinConexion();
        }
    } else if (reply->error() == QNetworkReply::NoError) {
        m_conexionDisponible = true;
    }
}
