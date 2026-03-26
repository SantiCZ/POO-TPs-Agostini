#include "HistorialRepository.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QUuid>

HistorialRepository::HistorialRepository(const QString& filePath)
    : m_filePath(filePath) {}

bool HistorialRepository::load() {
    QFile file(m_filePath);
    if (!file.exists()) return true;
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    m_acciones.clear();
    for (const QJsonValue& v : doc.array())
        m_acciones.append(fromJson(v.toObject()));
    return true;
}

bool HistorialRepository::save() {
    QDir().mkpath(QFileInfo(m_filePath).absolutePath());
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QJsonArray arr;
    for (const AccionHistorial& a : m_acciones)
        arr.append(toJson(a));
    file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void HistorialRepository::registrar(const QString& usuario, const QString& accion, const QString& detalle) {
    AccionHistorial a;
    a.id        = QUuid::createUuid().toString(QUuid::WithoutBraces);
    a.usuario   = usuario;
    a.accion    = accion;
    a.detalle   = detalle;
    a.timestamp = QDateTime::currentDateTime();
    m_acciones.prepend(a);
    if (m_acciones.size() > 200) m_acciones.removeLast();
    save();
}

QList<AccionHistorial> HistorialRepository::porUsuario(const QString& usuario) const {
    QList<AccionHistorial> res;
    for (const AccionHistorial& a : m_acciones)
        if (a.usuario == usuario) res.append(a);
    return res;
}

QList<AccionHistorial> HistorialRepository::todos() const { return m_acciones; }

AccionHistorial HistorialRepository::fromJson(const QJsonObject& o) {
    AccionHistorial a;
    a.id        = o["id"].toString();
    a.usuario   = o["usuario"].toString();
    a.accion    = o["accion"].toString();
    a.detalle   = o["detalle"].toString();
    a.timestamp = QDateTime::fromString(o["timestamp"].toString(), Qt::ISODate);
    return a;
}

QJsonObject HistorialRepository::toJson(const AccionHistorial& a) {
    QJsonObject o;
    o["id"]        = a.id;
    o["usuario"]   = a.usuario;
    o["accion"]    = a.accion;
    o["detalle"]   = a.detalle;
    o["timestamp"] = a.timestamp.toString(Qt::ISODate);
    return o;
}
