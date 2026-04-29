#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

// Logger de archivo de texto — complementa los logs en BD
class Logger
{
public:
    static Logger &instance();

    bool init(const QString &rutaArchivo = "accesos.log");
    void log(const QString &nivel, const QString &usuario, const QString &mensaje);
    void info (const QString &usuario, const QString &msg) { log("INFO",  usuario, msg); }
    void warn (const QString &usuario, const QString &msg) { log("WARN",  usuario, msg); }
    void error(const QString &usuario, const QString &msg) { log("ERROR", usuario, msg); }

private:
    Logger() = default;
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    QFile       m_archivo;
    QTextStream m_stream;
    QMutex      m_mutex;
};

#endif // LOGGER_H
