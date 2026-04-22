#include "logger.h"
#include <QMutexLocker>
#include <QDir>
#include <QDebug>

Logger& Logger::instancia()
{
    static Logger inst;
    return inst;
}

Logger::Logger()
    : m_inicializado(false)
{
}

Logger::~Logger()
{
    if (m_archivo.isOpen())
        m_archivo.close();
}

void Logger::inicializar(const QString& rutaArchivo)
{
    QMutexLocker locker(&m_mutex);

    // Crear directorio si no existe
    QFileInfo fi(rutaArchivo);
    QDir().mkpath(fi.absolutePath());

    m_archivo.setFileName(rutaArchivo);
    if (m_archivo.open(QIODevice::Append | QIODevice::Text)) {
        m_inicializado = true;
        QTextStream out(&m_archivo);
        out << "\n=== Sesión iniciada: "
            << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
            << " ===\n";
        out.flush();
    } else {
        qWarning() << "Logger: No se pudo abrir el archivo:" << rutaArchivo;
    }
}

void Logger::registrar(const QString& nivel, const QString& descripcion)
{
    QMutexLocker locker(&m_mutex);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString linea = QString("[%1] [%2] %3\n").arg(timestamp, nivel, descripcion);

    // Siempre imprimir en consola
    qDebug().noquote() << linea.trimmed();

    if (m_inicializado && m_archivo.isOpen()) {
        QTextStream out(&m_archivo);
        out << linea;
        out.flush();
    }
}

void Logger::info(const QString& descripcion)
{
    registrar("INFO", descripcion);
}

void Logger::error(const QString& descripcion)
{
    registrar("ERROR", descripcion);
}

void Logger::advertencia(const QString& descripcion)
{
    registrar("ADVERTENCIA", descripcion);
}
