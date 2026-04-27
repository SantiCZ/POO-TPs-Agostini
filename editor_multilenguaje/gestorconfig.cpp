#include "gestorconfig.h"

GestorConfig::GestorConfig()
    : m_usuario("admin"),
    m_clave("1234"),
    m_intentosMax(3),
    m_tiempoBloqueo(30),
    m_lenguajeDefecto("C++"),
    m_rutaExportacion("./exportaciones")
{
}

GestorConfig& GestorConfig::instancia()
{
    static GestorConfig inst;
    return inst;
}

void GestorConfig::cargar(const QString &rutaArchivo)
{
    QSettings settings(rutaArchivo, QSettings::IniFormat);

    m_usuario           = settings.value("Credenciales/usuario", "admin").toString();
    m_clave             = settings.value("Credenciales/clave", "1234").toString();
    m_intentosMax       = settings.value("Seguridad/intentos_max", 3).toInt();
    m_tiempoBloqueo     = settings.value("Seguridad/tiempo_bloqueo_segundos", 30).toInt();
    m_lenguajeDefecto   = settings.value("Editor/lenguaje_defecto", "C++").toString();
    m_rutaExportacion   = settings.value("Exportacion/ruta_exportacion", "./exportaciones").toString();
}

QString GestorConfig::usuario() const           { return m_usuario; }
QString GestorConfig::clave() const             { return m_clave; }
int     GestorConfig::intentosMax() const       { return m_intentosMax; }
int     GestorConfig::tiempoBloqueoSegundos() const { return m_tiempoBloqueo; }
QString GestorConfig::lenguajeDefecto() const   { return m_lenguajeDefecto; }
QString GestorConfig::rutaExportacion() const   { return m_rutaExportacion; }
