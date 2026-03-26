#pragma once
#include <QList>
#include <QString>
#include "AccionHistorial.h"

class HistorialRepository {
public:
    explicit HistorialRepository(const QString& filePath);

    bool                   load();
    bool                   save();
    void                   registrar(const QString& usuario, const QString& accion, const QString& detalle);
    QList<AccionHistorial> porUsuario(const QString& usuario) const;
    QList<AccionHistorial> todos() const;

private:
    QString                m_filePath;
    QList<AccionHistorial> m_acciones;

    static AccionHistorial fromJson(const QJsonObject& obj);
    static QJsonObject     toJson(const AccionHistorial& a);
};
