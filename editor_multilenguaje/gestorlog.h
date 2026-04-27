#ifndef GESTORLOG_H
#define GESTORLOG_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

// ============================================================
// GestorLog: Singleton para escritura de eventos en archivo
// ============================================================
class GestorLog
{
public:
    static GestorLog& instancia();

    void registrar(const QString &pantalla, const QString &descripcion);
    void setArchivoLog(const QString &ruta);

private:
    GestorLog();
    ~GestorLog();
    GestorLog(const GestorLog&) = delete;
    GestorLog& operator=(const GestorLog&) = delete;

    QString m_rutaLog;
};

#endif // GESTORLOG_H