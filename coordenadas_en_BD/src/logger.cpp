#include "logger.h"
#include <QDateTime>
#include <QMutexLocker>

Logger &Logger::instance()
{
    static Logger inst;
    return inst;
}

Logger::~Logger()
{
    if (m_archivo.isOpen()) m_archivo.close();
}

bool Logger::init(const QString &rutaArchivo)
{
    QMutexLocker lock(&m_mutex);
    m_archivo.setFileName(rutaArchivo);

    // append para no perder logs entre sesiones
    if (!m_archivo.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    m_stream.setDevice(&m_archivo);
    m_stream << "\n=== sesion iniciada: "
             << QDateTime::currentDateTime().toString(Qt::ISODate)
             << " ===\n";
    m_stream.flush();
    return true;
}

void Logger::log(const QString &nivel, const QString &usuario, const QString &mensaje)
{
    QMutexLocker lock(&m_mutex);
    if (!m_archivo.isOpen()) return;

    // nunca se registran contrasenas; solo usuario y evento
    QString nivelFmt = nivel.leftJustified(5, ' ');

    QString linea = QString("[%1] [%2] usuario='%3' | %4\n")
                        .arg(
                            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                            nivelFmt,
                            usuario,
                            mensaje
                            );

    m_stream << linea;
    m_stream.flush();
}