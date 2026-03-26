#pragma once
#include <QList>
#include <QString>
#include "TrabajoPractico.h"

class TPRepository {
public:
    explicit TPRepository(const QString& filePath);

    bool                   load();
    bool                   save();
    QList<TrabajoPractico> porUsuario(const QString& usuario) const;
    bool                   agregar(const TrabajoPractico& tp);
    bool                   actualizar(const TrabajoPractico& tp);
    bool                   eliminar(const QString& id);
    TrabajoPractico*       buscarPorId(const QString& id);

private:
    QString                m_filePath;
    QList<TrabajoPractico> m_tps;

    static TrabajoPractico fromJson(const QJsonObject& obj);
    static QJsonObject     toJson(const TrabajoPractico& tp);
};
