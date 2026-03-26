#pragma once
#include <QString>

struct Usuario {
    QString username;
    QString password;

    bool isValid() const { return !username.isEmpty() && !password.isEmpty(); }
};
