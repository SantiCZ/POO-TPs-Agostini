#ifndef GESTORCONFIG_H
#define GESTORCONFIG_H

#include <QString>
#include <QSettings>

class GestorConfig
{
public:
    static GestorConfig& instancia();

    void cargar(const QString &rutaArchivo);

    QString usuario() const;
    QString clave() const;
    int intentosMax() const;
    int tiempoBloqueoSegundos() const;
    QString lenguajeDefecto() const;
    QString rutaExportacion() const;

private:
    GestorConfig();
    GestorConfig(const GestorConfig&) = delete;
    GestorConfig& operator=(const GestorConfig&) = delete;

    QString m_usuario;
    QString m_clave;
    int     m_intentosMax;
    int     m_tiempoBloqueo;
    QString m_lenguajeDefecto;
    QString m_rutaExportacion;
};

#endif // GESTORCONFIG_H