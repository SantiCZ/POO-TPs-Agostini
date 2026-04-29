#ifndef PINTURA_H
#define PINTURA_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <QVector>
#include <QPixmap>
#include "admindb.h"

struct TrazoActivo {
    int    segmentoId    = -1;
    QColor color;
    int    grosor        = 3;
    QVector<QPoint> puntos;
    int    contadorPuntos = 0;
};

class Pintura : public QWidget
{
    Q_OBJECT

public:
    explicit Pintura(AdminDB *adminDB, QWidget *parent = nullptr);

    void   setColor(const QColor &c);
    void   setGrosor(int g);
    QColor colorActual()  const { return m_color;  }
    int    grosorActual() const { return m_grosor; }

    void cargarDesdeDB();

signals:
    void colorCambiado(const QColor &color);
    void grosorCambiado(int grosor);
    void statusMessage(const QString &msg);

protected:
    void paintEvent       (QPaintEvent  *event) override;
    void mousePressEvent  (QMouseEvent  *event) override;
    void mouseMoveEvent   (QMouseEvent  *event) override;
    void mouseReleaseEvent(QMouseEvent  *event) override;
    void wheelEvent       (QWheelEvent  *event) override;
    void keyPressEvent    (QKeyEvent    *event) override;
    void resizeEvent      (QResizeEvent *event) override;

private:
    void dibujarSegmento(const QPoint &desde, const QPoint &hasta);
    void deshacerUltimo();
    void borrarTodo();
    void redibujarDesdeHistorial();

    AdminDB        *m_adminDB;
    QPixmap         m_canvas;
    QColor          m_color;
    int             m_grosor;
    bool            m_dibujando;
    QPoint          m_ultimoPunto;
    TrazoActivo     m_trazoActivo;

    static constexpr int MAX_UNDO = 10;
    QVector<TrazoSegmento> m_historial;
    QVector<int>           m_pilaDeshacer;
};

#endif // PINTURA_H
