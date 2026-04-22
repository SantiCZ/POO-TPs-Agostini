#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

/**
 * Logger - Clase singleton para registro de eventos con fecha y descripción.
 * Escribe en archivo de log con timestamp para cada acción del sistema.
 */
class Logger
{
public:
    static Logger& instancia();

    void inicializar(const QString& rutaArchivo);
    void registrar(const QString& nivel, const QString& descripcion);
    void info(const QString& descripcion);
    void error(const QString& descripcion);
    void advertencia(const QString& descripcion);

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    QFile   m_archivo;
    QMutex  m_mutex;
    bool    m_inicializado;
};

#endif // LOGGER_H
