#pragma once
#include <QList>
#include <QString>
#include "Usuario.h"

class UsuarioRepository {
public:
    explicit UsuarioRepository(const QString& filePath);

    bool           load();
    bool           save();
    bool           existe(const QString& username) const;
    bool           validarCredenciales(const QString& username, const QString& password) const;
    bool           agregar(const Usuario& usuario);
    QList<Usuario> todos() const;

private:
    QString        m_filePath;
    QList<Usuario> m_usuarios;

    void           seedDefaultUsers();
};
