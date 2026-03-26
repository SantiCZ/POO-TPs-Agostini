#include "UsuarioRepository.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>

UsuarioRepository::UsuarioRepository(const QString& filePath)
    : m_filePath(filePath) {}

bool UsuarioRepository::load() {
    QFile file(m_filePath);
    if (!file.exists()) {
        seedDefaultUsers();
        return save();
    }
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    m_usuarios.clear();
    for (const QJsonValue& v : doc.array()) {
        QJsonObject o = v.toObject();
        Usuario u;
        u.username = o["username"].toString();
        u.password = o["password"].toString();
        if (u.isValid()) m_usuarios.append(u);
    }
    return true;
}

bool UsuarioRepository::save() {
    QDir().mkpath(QFileInfo(m_filePath).absolutePath());
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QJsonArray arr;
    for (const Usuario& u : m_usuarios) {
        QJsonObject o;
        o["username"] = u.username;
        o["password"] = u.password;
        arr.append(o);
    }
    file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool UsuarioRepository::existe(const QString& username) const {
    for (const Usuario& u : m_usuarios)
        if (u.username == username) return true;
    return false;
}

bool UsuarioRepository::validarCredenciales(const QString& username, const QString& password) const {
    for (const Usuario& u : m_usuarios)
        if (u.username == username && u.password == password) return true;
    return false;
}

bool UsuarioRepository::agregar(const Usuario& usuario) {
    if (existe(usuario.username)) return false;
    m_usuarios.append(usuario);
    return save();
}

QList<Usuario> UsuarioRepository::todos() const { return m_usuarios; }

void UsuarioRepository::seedDefaultUsers() {
    m_usuarios.clear();
    m_usuarios.append(Usuario{"admin",  "admin"});
    m_usuarios.append(Usuario{"alumno", "1234"});
}
