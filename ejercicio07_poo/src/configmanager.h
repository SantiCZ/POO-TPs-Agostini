#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QSettings>

/**
 * ConfigManager - Lee la configuración desde config.ini.
 * Provee acceso a API key, ciudad, credenciales, proxy e imagen.
 */
class ConfigManager
{
public:
    static ConfigManager& instancia();

    bool cargar(const QString& rutaArchivo);

    // API clima
    QString apiKey()     const;
    QString apiUrl()     const;
    QString ciudad()     const;
    QString unidades()   const;

    // Proxy
    QString proxyHost()  const;
    int     proxyPort()  const;
    QString proxyUser()  const;
    QString proxyPass()  const;
    bool    usaProxy()   const;

    // Imagen de fondo
    QString imagenUrl()       const;
    QString imagenCachePath() const;

    // Autenticación
    QString usuarioValido()   const;
    QString passwordValido()  const;
    int     maxIntentos()     const;
    int     bloqueoSegundos() const;

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    QSettings* m_settings = nullptr;
};

#endif // CONFIGMANAGER_H
