#include "TPRepository.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QUuid>

TPRepository::TPRepository(const QString& filePath)
    : m_filePath(filePath) {}

bool TPRepository::load() {
    QFile file(m_filePath);
    if (!file.exists()) return true;
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    m_tps.clear();
    for (const QJsonValue& v : doc.array())
        m_tps.append(fromJson(v.toObject()));
    return true;
}

bool TPRepository::save() {
    QDir().mkpath(QFileInfo(m_filePath).absolutePath());
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QJsonArray arr;
    for (const TrabajoPractico& tp : m_tps)
        arr.append(toJson(tp));
    file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QList<TrabajoPractico> TPRepository::porUsuario(const QString& usuario) const {
    QList<TrabajoPractico> res;
    for (const TrabajoPractico& tp : m_tps)
        if (tp.usuarioOwner == usuario) res.append(tp);
    return res;
}

bool TPRepository::agregar(const TrabajoPractico& tp) {
    m_tps.append(tp);
    return save();
}

bool TPRepository::actualizar(const TrabajoPractico& tp) {
    for (int i = 0; i < m_tps.size(); ++i) {
        if (m_tps[i].id == tp.id) { m_tps[i] = tp; return save(); }
    }
    return false;
}

bool TPRepository::eliminar(const QString& id) {
    for (int i = 0; i < m_tps.size(); ++i) {
        if (m_tps[i].id == id) { m_tps.removeAt(i); return save(); }
    }
    return false;
}

TrabajoPractico* TPRepository::buscarPorId(const QString& id) {
    for (TrabajoPractico& tp : m_tps)
        if (tp.id == id) return &tp;
    return nullptr;
}

TrabajoPractico TPRepository::fromJson(const QJsonObject& o) {
    TrabajoPractico tp;
    tp.id           = o["id"].toString();
    tp.usuarioOwner = o["usuario"].toString();
    tp.nombre       = o["nombre"].toString();
    tp.descripcion  = o["descripcion"].toString();
    tp.notas        = o["notas"].toString();
    tp.estado       = TrabajoPractico::estadoFromString(o["estado"].toString());
    tp.prioridad    = TrabajoPractico::prioridadFromString(o["prioridad"].toString());
    tp.fechaEntrega = QDate::fromString(o["fechaEntrega"].toString(), Qt::ISODate);
    tp.createdAt    = QDateTime::fromString(o["createdAt"].toString(), Qt::ISODate);
    tp.updatedAt    = QDateTime::fromString(o["updatedAt"].toString(), Qt::ISODate);
    return tp;
}

QJsonObject TPRepository::toJson(const TrabajoPractico& tp) {
    QJsonObject o;
    o["id"]          = tp.id;
    o["usuario"]     = tp.usuarioOwner;
    o["nombre"]      = tp.nombre;
    o["descripcion"] = tp.descripcion;
    o["notas"]       = tp.notas;
    o["estado"]      = TrabajoPractico::estadoToString(tp.estado);
    o["prioridad"]   = TrabajoPractico::prioridadToString(tp.prioridad);
    o["fechaEntrega"]= tp.fechaEntrega.toString(Qt::ISODate);
    o["createdAt"]   = tp.createdAt.toString(Qt::ISODate);
    o["updatedAt"]   = tp.updatedAt.toString(Qt::ISODate);
    return o;
}
