#include "admindb.h"
#include <QCryptographicHash>

// ─────────────────────────────────────────────────────────────────
AdminDB::AdminDB(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "conexion_principal");
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::conectar(QString archivoSqlite)
{
    db.setDatabaseName(archivoSqlite);

    if (db.open()) {
        qDebug() << "conexion exitosa:" << archivoSqlite;

        QSqlQuery q(db);
        q.exec("PRAGMA foreign_keys = ON");
        q.exec("PRAGMA journal_mode = WAL");

        crearTablas();
        seedUsuarios();
        return true;
    }

    qDebug() << "no se pudo abrir la base:" << db.lastError().text();
    return false;
}

// ─────────────────────────────────────────────────────────────────
QSqlDatabase& AdminDB::getDB()
{
    return db;
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::crearTablas()
{
    QSqlQuery query(db);

    query.exec(
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  usuario VARCHAR(30) NOT NULL UNIQUE,"
        "  clave VARCHAR(64) NOT NULL,"
        "  nombre VARCHAR(50),"
        "  apellido VARCHAR(50),"
        "  mail VARCHAR(50),"
        "  activo INTEGER NOT NULL DEFAULT 1"
        ")"
        );

    query.exec(
        "CREATE TABLE IF NOT EXISTS trazos_segmentos ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  color_r INTEGER NOT NULL,"
        "  color_g INTEGER NOT NULL,"
        "  color_b INTEGER NOT NULL,"
        "  grosor INTEGER NOT NULL DEFAULT 3"
        ")"
        );

    query.exec(
        "CREATE TABLE IF NOT EXISTS trazos_puntos ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  segmento_id INTEGER NOT NULL "
        "    REFERENCES trazos_segmentos(id) ON DELETE CASCADE,"
        "  orden INTEGER NOT NULL,"
        "  x INTEGER NOT NULL,"
        "  y INTEGER NOT NULL"
        ")"
        );

    query.exec(
        "CREATE INDEX IF NOT EXISTS idx_puntos_seg "
        "ON trazos_puntos(segmento_id, orden)"
        );

    query.exec(
        "CREATE TABLE IF NOT EXISTS logs_acceso ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  evento VARCHAR(200) NOT NULL,"
        "  exitoso INTEGER NOT NULL DEFAULT 0,"
        "  timestamp TEXT NOT NULL DEFAULT (datetime('now'))"
        ")"
        );

    return true;
}

// ─────────────────────────────────────────────────────────────────
void AdminDB::seedUsuarios()
{
    QSqlQuery query(db);

    query.exec("SELECT COUNT(*) FROM usuarios");
    if (query.next() && query.value(0).toInt() > 0)
        return;

    struct {
        const char *usuario;
        const char *clave;
        const char *nombre;
        const char *apellido;
    } seeds[] = {
                 {"admin", "admin123", "Carlos", "Gomez"},
                 {"user",  "user123",  "Ana",    "Lopez"},
                 {"pepe",  "pepe456",  "Pepe",   "Argento"},
                 };

    for (auto &s : seeds) {
        QString hash = QCryptographicHash::hash(
                           QByteArray(s.clave),
                           QCryptographicHash::Sha256
                           ).toHex();

        QSqlQuery insert(db);
        insert.prepare(
            "INSERT INTO usuarios (usuario, clave, nombre, apellido) "
            "VALUES (:usuario, :clave, :nombre, :apellido)"
            );

        insert.bindValue(":usuario", s.usuario);
        insert.bindValue(":clave", hash);
        insert.bindValue(":nombre", s.nombre);
        insert.bindValue(":apellido", s.apellido);

        insert.exec();
    }
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::validarUsuario(const QString &usuario, const QString &claveHash)
{
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare(
        "SELECT nombre, apellido FROM usuarios "
        "WHERE usuario = :usuario AND clave = :clave AND activo = 1"
        );

    query.bindValue(":usuario", usuario);
    query.bindValue(":clave", claveHash);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    if (query.next()) {
        qDebug() << "nombre=" << query.value("nombre").toString();
        qDebug() << "apellido=" << query.value("apellido").toString();
        return true;
    }

    return false;
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::registrar(QString evento)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO logs_acceso (evento) VALUES (:evento)");
    query.bindValue(":evento", evento);

    return query.exec();
}

// ─────────────────────────────────────────────────────────────────
int AdminDB::guardarSegmento(const QColor &color, int grosor)
{
    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO trazos_segmentos (color_r, color_g, color_b, grosor) "
        "VALUES (:r, :g, :b, :grosor)"
        );

    query.bindValue(":r", color.red());
    query.bindValue(":g", color.green());
    query.bindValue(":b", color.blue());
    query.bindValue(":grosor", grosor);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::agregarPunto(int segmentoId, const QPoint &punto, int orden)
{
    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO trazos_puntos (segmento_id, orden, x, y) "
        "VALUES (:id, :orden, :x, :y)"
        );

    query.bindValue(":id", segmentoId);
    query.bindValue(":orden", orden);
    query.bindValue(":x", punto.x());
    query.bindValue(":y", punto.y());

    return query.exec();
}

// ─────────────────────────────────────────────────────────────────
QVector<TrazoSegmento> AdminDB::cargarTrazos()
{
    QVector<TrazoSegmento> resultado;

    QSqlQuery qSeg(db);
    qSeg.exec("SELECT id, color_r, color_g, color_b, grosor FROM trazos_segmentos ORDER BY id");

    while (qSeg.next()) {
        TrazoSegmento seg;
        seg.id = qSeg.value("id").toInt();
        seg.color = QColor(
            qSeg.value("color_r").toInt(),
            qSeg.value("color_g").toInt(),
            qSeg.value("color_b").toInt()
            );
        seg.grosor = qSeg.value("grosor").toInt();
        resultado.append(seg);
    }

    for (auto &seg : resultado) {
        QSqlQuery qPts(db);
        qPts.prepare("SELECT x, y FROM trazos_puntos WHERE segmento_id = :id ORDER BY orden");
        qPts.bindValue(":id", seg.id);
        qPts.exec();

        while (qPts.next()) {
            seg.puntos.append(QPoint(
                qPts.value(0).toInt(),
                qPts.value(1).toInt()
                ));
        }
    }

    return resultado;
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::borrarTodosTrazos()
{
    QSqlQuery query(db);
    return query.exec("DELETE FROM trazos_segmentos");
}

// ─────────────────────────────────────────────────────────────────
bool AdminDB::borrarSegmento(int segmentoId)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM trazos_segmentos WHERE id = :id");
    query.bindValue(":id", segmentoId);
    return query.exec();
}