#include "clima.h"
#include "configmanager.h"
#include "logger.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>

Clima::Clima(NetworkManager* nm, QWidget* parent)
    : Pantalla(parent)
    , m_nm(nm)
    , m_timer(new QTimer(this))
    , m_replyClima(nullptr)
    , m_temperatura(0.0)
    , m_descripcion("---")
    , m_ciudad("---")
    , m_icono("01d")
    , m_humedad(0)
    , m_offline(false)
{
}

void Clima::inicializar()
{
    // Actualización periódica cada 10 minutos
    connect(m_timer, &QTimer::timeout,
            this,    &Clima::onActualizacionPeriodica);
    m_timer->start(600000);

    // Detectar pérdida de conexión
    connect(m_nm, &NetworkManager::sinConexion,
            this, &Clima::onSinConexion);

    Logger::instancia().info("Clima: Inicializado, consulta cada 10 minutos");
    consultarClima();
}

void Clima::actualizarUI()
{
    // No tiene UI propia; delega al Login mediante señales
}

void Clima::consultarClima()
{
    ConfigManager& cfg = ConfigManager::instancia();
    QString apiKey = cfg.apiKey();

    if (apiKey.isEmpty() || apiKey == "TU_API_KEY_AQUI") {
        Logger::instancia().advertencia("Clima: API key no configurada, usando datos offline");
        usarDatosOffline();
        return;
    }

    // Construcción manual de URL con parámetros (sin SDK)
    QUrl url(cfg.apiUrl());
    QUrlQuery query;
    query.addQueryItem("q",     cfg.ciudad());
    query.addQueryItem("units", cfg.unidades());
    query.addQueryItem("lang",  "es");
    query.addQueryItem("appid", apiKey);
    url.setQuery(query);

    Logger::instancia().info("Clima: Consultando -> " + url.toString());

    // Cancelar request previo si existe
    if (m_replyClima && m_replyClima->isRunning()) {
        m_replyClima->abort();
    }

    m_replyClima = m_nm->get(url);

    // Connect tal como se vio en clase
    connect(m_replyClima, &QNetworkReply::finished,
            this, [this]() { onRespuestaClima(m_replyClima); });
}

void Clima::onRespuestaClima(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QString err = reply->errorString();
        Logger::instancia().error("Clima: Error de red -> " + err);
        emit errorClima("No se pudo obtener el clima:\n" + err + "\n\nUsando datos de ejemplo.");
        usarDatosOffline();
        reply->deleteLater();
        return;
    }

    QByteArray datos = reply->readAll();
    Logger::instancia().info("Clima: Respuesta recibida (" + QString::number(datos.size()) + " bytes)");
    parsearRespuesta(datos);
    reply->deleteLater();
}

void Clima::parsearRespuesta(const QByteArray& datos)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(datos, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        Logger::instancia().error("Clima: Error al parsear JSON -> " + parseError.errorString());
        emit errorClima("Respuesta inválida del servidor de clima.");
        usarDatosOffline();
        return;
    }

    QJsonObject root = doc.object();

    // Validación de datos recibidos
    if (!root.contains("main") || !root.contains("weather")) {
        Logger::instancia().error("Clima: Respuesta JSON sin campos esperados");
        // Verificar código de error de la API
        if (root.contains("message")) {
            emit errorClima("API de clima: " + root["message"].toString());
        } else {
            emit errorClima("Respuesta inesperada del servidor de clima.");
        }
        usarDatosOffline();
        return;
    }

    QJsonObject main    = root["main"].toObject();
    QJsonArray  weather = root["weather"].toArray();
    QJsonObject wObj    = weather.isEmpty() ? QJsonObject() : weather[0].toObject();

    m_temperatura = main["temp"].toDouble();
    m_humedad     = main["humidity"].toInt();
    m_descripcion = wObj["description"].toString("sin datos");
    m_icono       = wObj["icon"].toString("01d");
    m_ciudad      = root["name"].toString("Córdoba");
    m_offline     = false;

    // Capitalizar descripción
    if (!m_descripcion.isEmpty())
        m_descripcion[0] = m_descripcion[0].toUpper();

    Logger::instancia().info(QString("Clima: %1°C, %2, humedad %3%")
                             .arg(m_temperatura, 0, 'f', 1)
                             .arg(m_descripcion)
                             .arg(m_humedad));

    emit climaActualizado(m_temperatura, m_descripcion, m_ciudad, m_icono, m_humedad);
}

void Clima::usarDatosOffline()
{
    m_offline     = true;
    m_temperatura = 22.0;
    m_descripcion = "Parcialmente nublado (offline)";
    m_ciudad      = "Córdoba";
    m_icono       = "02d";
    m_humedad     = 55;

    Logger::instancia().advertencia("Clima: Usando datos simulados (modo offline)");
    emit climaActualizado(m_temperatura, m_descripcion, m_ciudad, m_icono, m_humedad);
}

void Clima::onActualizacionPeriodica()
{
    Logger::instancia().info("Clima: Actualización periódica");
    consultarClima();
}

void Clima::onSinConexion()
{
    Logger::instancia().advertencia("Clima: Sin conexión, activando modo offline");
    usarDatosOffline();
}
