#include "gestorlog.h"
#include <QDir>

GestorLog::GestorLog()
    : m_rutaLog("eventos.log")
{
}

GestorLog::~GestorLog()
{
}

GestorLog& GestorLog::instancia()
{
    static GestorLog inst;
    return inst;
}

void GestorLog::setArchivoLog(const QString &ruta)
{
    m_rutaLog = ruta;
}

void GestorLog::registrar(const QString &pantalla, const QString &descripcion)
{
    QFile archivo(m_rutaLog);
    if (archivo.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&archivo);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        stream << "[" << timestamp << "] [" << pantalla << "] " << descripcion << "\n";
        archivo.close();
    }
}
