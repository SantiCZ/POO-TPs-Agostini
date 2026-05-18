#include "trex.h"

TRex::TRex(QObject *parent)
    : QObject(parent),
    m_x(BASE_X),
    m_y(GROUND_Y),
    m_velocityY(0),
    m_state(Running),
    m_animFrame(0)
{
    // timer de gravedad que corre cada 16 ms
    // equivalente a unos 60 fps
    m_gravityTimer = new QTimer(this);
    connect(m_gravityTimer, SIGNAL(timeout()), this, SLOT(slot_gravity()));
    m_gravityTimer->start(16);

    // timer para animar los pasos del dinosaurio
    m_animTimer = new QTimer(this);
    connect(m_animTimer, SIGNAL(timeout()), this, SLOT(slot_animate()));
    m_animTimer->start(120);
}

// ------------------------------------------------------------------
// salto permitido solo cuando esta en el suelo
// ------------------------------------------------------------------
void TRex::jump()
{
    if (m_state != Jumping) {
        m_state     = Jumping;
        m_velocityY = JUMP_VELOCITY;
    }
}

// ------------------------------------------------------------------
// cambia al estado agachado
// ------------------------------------------------------------------
void TRex::duck()
{
    if (m_state != Jumping) {
        m_state = Ducking;
    }
}

// ------------------------------------------------------------------
// vuelve al estado normal
// ------------------------------------------------------------------
void TRex::stand()
{
    if (m_state == Ducking) {
        m_state = Running;
    }
}

// ------------------------------------------------------------------
// mueve el dinosaurio hacia adelante
// ------------------------------------------------------------------
void TRex::moveForward()
{
    m_x += 10;

    // limita el movimiento maximo hacia adelante
    if (m_x > BASE_X + 80)
        m_x = BASE_X + 80;
}

// ------------------------------------------------------------------
// mueve el dinosaurio hacia atras
// ------------------------------------------------------------------
void TRex::moveBackward()
{
    m_x -= 10;

    // limita el movimiento maximo hacia atras
    if (m_x < BASE_X - 40)
        m_x = BASE_X - 40;
}

// ------------------------------------------------------------------
// aplica gravedad durante el salto
// ------------------------------------------------------------------
void TRex::slot_gravity()
{
    if (m_state == Jumping) {

        // actualiza la posicion vertical
        m_y += m_velocityY;

        // aumenta la velocidad por efecto de gravedad
        m_velocityY += GRAVITY;

        // detecta cuando vuelve al suelo
        if (m_y >= GROUND_Y) {
            m_y         = GROUND_Y;
            m_velocityY = 0;
            m_state     = Running;
        }
    }
}

// ------------------------------------------------------------------
// alterna entre frames de animacion
// ------------------------------------------------------------------
void TRex::slot_animate()
{
    m_animFrame = (m_animFrame + 1) % 2;
}

// ------------------------------------------------------------------
// devuelve el rectangulo de colision
// ------------------------------------------------------------------
QRect TRex::boundingRect() const
{
    int margin = 6;

    // reduce el area de colision con un margen interno
    int w = width()  - margin * 2;
    int h = height() - margin * 2;

    // ajusta la posicion cuando esta agachado
    int yPos = (m_state == Ducking)
                   ? (GROUND_Y + NORMAL_HEIGHT - DUCK_HEIGHT + margin)
                   : (m_y + margin);

    return QRect(m_x + margin, yPos, w, h);
}

// ------------------------------------------------------------------
// devuelve el ancho actual segun el estado
// ------------------------------------------------------------------
int TRex::width() const
{
    return (m_state == Ducking) ? DUCK_WIDTH : NORMAL_WIDTH;
}

// ------------------------------------------------------------------
// devuelve la altura actual segun el estado
// ------------------------------------------------------------------
int TRex::height() const
{
    return (m_state == Ducking) ? DUCK_HEIGHT : NORMAL_HEIGHT;
}