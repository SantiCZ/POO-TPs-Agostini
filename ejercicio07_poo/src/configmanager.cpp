#include "configmanager.h"
#include "logger.h"
#include <QFileInfo>

ConfigManager& ConfigManager::instancia()
{
    static ConfigManager inst;
    return inst;
}

bool ConfigManager::cargar(const QString& rutaArchivo)
{
    if (!QFileInfo::exists(rutaArchivo)) {
        Logger::instancia().error("ConfigManager: Archivo no encontrado: " + rutaArchivo);
        return false;
    }
    m_settings = new QSettings(rutaArchivo, QSettings::IniFormat);
    Logger::instancia().info("ConfigManager: Configuración cargada desde " + rutaArchivo);
    return true;
}

QString ConfigManager::apiKey() const
{
    return m_settings ? m_settings->value("API/apikey", "").toString() : "";
}

QString ConfigManager::apiUrl() const
{
    return m_settings
        ? m_settings->value("API/url", "https://api.openweathermap.org/data/2.5/weather").toString()
        : "https://api.openweathermap.org/data/2.5/weather";
}

QString ConfigManager::ciudad() const
{
    return m_settings ? m_settings->value("General/ciudad", "Cordoba,AR").toString() : "Cordoba,AR";
}

QString ConfigManager::unidades() const
{
    return m_settings ? m_settings->value("General/unidades", "metric").toString() : "metric";
}

QString ConfigManager::proxyHost() const
{
    return m_settings ? m_settings->value("Red/proxy_host", "").toString() : "";
}

int ConfigManager::proxyPort() const
{
    return m_settings ? m_settings->value("Red/proxy_port", 0).toInt() : 0;
}

QString ConfigManager::proxyUser() const
{
    return m_settings ? m_settings->value("Red/proxy_user", "").toString() : "";
}

QString ConfigManager::proxyPass() const
{
    return m_settings ? m_settings->value("Red/proxy_pass", "").toString() : "";
}

bool ConfigManager::usaProxy() const
{
    return !proxyHost().isEmpty() && proxyPort() > 0;
}

QString ConfigManager::imagenUrl() const
{
    return m_settings
        ? m_settings->value("Imagen/url",
              "https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=1920&q=80").toString()
        : "";
}

QString ConfigManager::imagenCachePath() const
{
    return m_settings ? m_settings->value("Imagen/cache_path", "./cache/fondo.jpg").toString()
                      : "./cache/fondo.jpg";
}

QString ConfigManager::usuarioValido() const
{
    return m_settings ? m_settings->value("Auth/usuario", "admin").toString() : "admin";
}

QString ConfigManager::passwordValido() const
{
    return m_settings ? m_settings->value("Auth/password", "1234").toString() : "1234";
}

int ConfigManager::maxIntentos() const
{
    return m_settings ? m_settings->value("Auth/max_intentos", 3).toInt() : 3;
}

int ConfigManager::bloqueoSegundos() const
{
    return m_settings ? m_settings->value("Auth/bloqueo_segundos", 30).toInt() : 30;
}
