#include "exportadorjpg.h"
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QDateTime>
#include <QDir>
#include <QStringList>

ExportadorJpg::ExportadorJpg()
{
}

QString ExportadorJpg::exportar(const QString &codigo,
                                 const QString &lenguaje,
                                 const QString &rutaDestino)
{
    QStringList lineas = codigo.split('\n');
    int cantLineas = lineas.size();

    // Fuente monoespaciada para renderizar código
    QFont fuente("Courier New", 11);
    fuente.setFixedPitch(true);
    QFontMetrics fm(fuente);

    const int margenX = 40;
    const int margenY = 60;
    const int alturaLinea = fm.height() + 4;
    const int numeroAncho = 50; // espacio para número de línea

    // Calcular ancho máximo de línea
    int anchoMax = 600;
    for (const QString &l : lineas) {
        int w = fm.horizontalAdvance(l) + margenX * 2 + numeroAncho;
        if (w > anchoMax) anchoMax = w;
    }
    anchoMax = qMax(anchoMax, 800);

    int altura = margenY * 2 + cantLineas * alturaLinea + 80; // +80 para encabezado

    QPixmap pixmap(anchoMax, altura);
    pixmap.fill(QColor("#1a1a2e")); // fondo oscuro

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setFont(fuente);

    // ---- Encabezado ----
    QFont fuenteTitulo("Segoe UI", 13, QFont::Bold);
    painter.setFont(fuenteTitulo);
    painter.setPen(QColor("#e0e0ff"));
    QString titulo = QString("Editor Multilenguaje  |  Lenguaje: %1  |  Exportado: %2")
                         .arg(lenguaje)
                         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));
    painter.drawText(QRect(margenX, 18, anchoMax - margenX * 2, 36),
                     Qt::AlignLeft | Qt::AlignVCenter, titulo);

    // Línea separadora
    painter.setPen(QPen(QColor("#3030aa"), 1));
    painter.drawLine(margenX, 54, anchoMax - margenX, 54);

    // ---- Líneas de código ----
    painter.setFont(fuente);

    int y = margenY + 30;
    for (int i = 0; i < cantLineas; i++) {
        // Número de línea
        painter.setPen(QColor("#4a4a7a"));
        painter.drawText(QRect(margenX, y, numeroAncho - 10, alturaLinea),
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(i + 1));

        // Texto de la línea
        painter.setPen(QColor("#c8c8ff"));
        painter.drawText(margenX + numeroAncho, y,
                         anchoMax - margenX * 2 - numeroAncho,
                         alturaLinea,
                         Qt::AlignLeft | Qt::AlignVCenter,
                         lineas[i]);
        y += alturaLinea;
    }

    painter.end();

    // ---- Guardar archivo ----
    QDir().mkpath(rutaDestino);
    QString nombreArchivo = QString("codigo_%1.jpg")
                                .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString rutaCompleta = rutaDestino + "/" + nombreArchivo;

    pixmap.save(rutaCompleta, "JPG", 90);
    return rutaCompleta;
}
