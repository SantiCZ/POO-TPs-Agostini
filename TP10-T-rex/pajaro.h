#ifndef PAJARO_H
#define PAJARO_H

#include <QObject>
#include <QRect>
#include <QTimer>

// -------------------------------------------------------
// clase pajaro
// cada instancia representa un obstaculo volador
// cada pajaro tiene su propio qtimer independiente
// para controlar el movimiento horizontal
// -------------------------------------------------------
class Pajaro : public QObject
{
    Q_OBJECT

public:
    // tipos de pajaro segun altura
    enum TipoPajaro {
        Alto,
        Medio,
        Bajo
    };

    explicit Pajaro(int startX, int windowWidth, TipoPajaro tipo, QObject *parent = nullptr);

    void setSpeed(int speed);

    QRect boundingRect() const;
    int x() const { return m_x; }
    int y() const { return m_y; }
    int animFrame() const { return m_animFrame; }

    // verifica si el pajaro salio de pantalla
    bool isOffScreen() const { return m_x + WIDTH < 0; }

    TipoPajaro tipo() const { return m_tipo; }

    static const int WIDTH  = 46;
    static const int HEIGHT = 30;

signals:
    // se emite cuando el pajaro cambia de posicion
    void moved();

private slots:
    void slot_move();
    void slot_animate();

private:
    int        m_x;
    int        m_y;
    int        m_speed;
    int        m_animFrame;
    TipoPajaro m_tipo;

    // timer de movimiento horizontal
    QTimer *m_moveTimer;

    // timer para animacion de alas
    QTimer *m_animTimer;
};

#endif // pajaro_h