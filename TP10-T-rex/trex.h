#ifndef TREX_H
#define TREX_H

#include <QObject>
#include <QRect>
#include <QTimer>

// -------------------------------------------------------
// clase trex
// representa al dinosaurio controlado por el jugador
// hereda de qobject para poder usar senales y slots
// gestiona los estados: correr, saltar y agacharse
// -------------------------------------------------------
class TRex : public QObject
{
    Q_OBJECT

public:
    // estados del dinosaurio
    enum State {
        Running,
        Jumping,
        Ducking
    };

    explicit TRex(QObject *parent = nullptr);

    // acciones del jugador
    void jump();
    void duck();
    void stand();
    void moveForward();
    void moveBackward();

    // getters de estado y posicion
    QRect boundingRect() const;
    State state() const { return m_state; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const;
    int height() const;
    int animFrame() const { return m_animFrame; }

    // constantes de posicion y dimensiones
    // usadas tambien desde gamewindow
    static const int GROUND_Y      = 280;
    static const int BASE_X        = 80;
    static const int DUCK_HEIGHT   = 30;
    static const int NORMAL_HEIGHT = 50;
    static const int NORMAL_WIDTH  = 44;
    static const int DUCK_WIDTH    = 60;

private slots:
    void slot_gravity();
    void slot_animate();

private:
    int   m_x;
    int   m_y;
    int   m_velocityY;
    State m_state;
    int   m_animFrame;

    QTimer *m_gravityTimer;   // aplica gravedad durante el salto
    QTimer *m_animTimer;      // anima los pasos del dinosaurio

    static const int JUMP_VELOCITY = -23;  // mientras mas negativo mas alto salta
    static const int GRAVITY       = 2;    // menos gravedad mas tiempo en el aire
};

#endif // trex_h