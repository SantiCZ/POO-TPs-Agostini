#pragma once
#include <QString>
#include <QDateTime>

enum class EstadoTP {
    Pendiente,
    EnCurso,
    Entregado,
    Vencido
};

enum class PrioridadTP {
    Alta,
    Media,
    Baja
};

struct TrabajoPractico {
    QString     id;
    QString     usuarioOwner;
    QString     nombre;
    QString     descripcion;
    QString     notas;
    QDate       fechaEntrega;
    EstadoTP    estado    = EstadoTP::Pendiente;
    PrioridadTP prioridad = PrioridadTP::Media;
    QDateTime   createdAt;
    QDateTime   updatedAt;

    static QString    estadoToString(EstadoTP e);
    static EstadoTP   estadoFromString(const QString& s);
    static QString    prioridadToString(PrioridadTP p);
    static PrioridadTP prioridadFromString(const QString& s);

    // Listas para combos
    static QStringList estadosList()    { return {"Pendiente","En curso","Entregado","Vencido"}; }
    static QStringList prioridadesList(){ return {"Alta","Media","Baja"}; }
};
