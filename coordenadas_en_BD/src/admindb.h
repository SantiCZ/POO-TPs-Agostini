#ifndef ADMINDB_H
#define ADMINDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QString>
#include <QDebug>

// representa un segmento de trazo con todos sus puntos
struct TrazoSegmento {
    int            id;
    QColor         color;
    int            grosor;
    QVector<QPoint> puntos;
};

class AdminDB : public QObject
{
    Q_OBJECT

public:
    explicit AdminDB(QObject *parent = nullptr);

    bool conectar(QString archivoSqlite);

    // devolver referencia (mejor que copia)
    QSqlDatabase& getDB();

    // usuarios
    bool validarUsuario(const QString &usuario, const QString &claveHash);
    void seedUsuarios();

    // trazos
    int  guardarSegmento(const QColor &color, int grosor);
    bool agregarPunto(int segmentoId, const QPoint &punto, int orden);
    QVector<TrazoSegmento> cargarTrazos();
    bool borrarTodosTrazos();
    bool borrarSegmento(int segmentoId);

    // logs
    bool registrar(QString evento);

private:
    QSqlDatabase db;
    bool crearTablas();
};

#endif // ADMINDB_H