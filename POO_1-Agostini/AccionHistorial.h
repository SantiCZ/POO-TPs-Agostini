#pragma once
#include <QString>
#include <QDateTime>

struct AccionHistorial {
    QString   id;
    QString   usuario;
    QString   accion;
    QString   detalle;
    QDateTime timestamp;
};
