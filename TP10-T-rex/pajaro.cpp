#include "pajaro.h"

// alturas segun el tipo de pajaro
// coordenada y tomada desde la parte superior
static const int ALTURA_ALTO  = 200;
static const int ALTURA_MEDIO = 240;
static const int ALTURA_BAJO  = 265;

Pajaro::Pajaro(int startX, int /*windowWidth*/, TipoPajaro tipo, QObject *parent)
    : QObject(parent),
    m_x(startX),
    m_speed(5),
    m_animFrame(0),
    m_tipo(tipo)
{
    // asigna la altura segun el tipo de pajaro
    switch (tipo) {
    case Alto:
        m_y = ALTURA_ALTO;
        break;

    case Medio:
        m_y = ALTURA_MEDIO;
        break;

    case Bajo:
        m_y = ALTURA_BAJO;
        break;
    }

    // timer propio para mover el pajaro
    // se ejecuta cada 16 ms
    m_moveTimer = new QTimer(this);
    connect(m_moveTimer, SIGNAL(timeout()), this, SLOT(slot_move()));
    m_moveTimer->start(16);

    // timer para animar el movimiento de alas
    m_animTimer = new QTimer(this);
    connect(m_animTimer, SIGNAL(timeout()), this, SLOT(slot_animate()));
    m_animTimer->start(150);
}

// ------------------------------------------------------------------
// cambia la velocidad del pajaro
// usada para aumentar la dificultad
// ------------------------------------------------------------------
void Pajaro::setSpeed(int speed)
{
    m_speed = speed;
}

// ------------------------------------------------------------------
// mueve el pajaro hacia la izquierda
// ------------------------------------------------------------------
void Pajaro::slot_move()
{
    m_x -= m_speed;

    // notifica que cambio la posicion
    emit moved();
}

// ------------------------------------------------------------------
// alterna el frame de animacion de alas
// ------------------------------------------------------------------
void Pajaro::slot_animate()
{
    m_animFrame = (m_animFrame + 1) % 2;
}

// ------------------------------------------------------------------
// devuelve el rectangulo de colision
// ------------------------------------------------------------------
QRect Pajaro::boundingRect() const
{
    int margin = 5;

    // aplica margen interno para colisiones mas justas
    return QRect(m_x + margin,
                 m_y + margin,
                 WIDTH - margin * 2,
                 HEIGHT - margin * 2);
}