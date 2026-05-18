#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include "trex.h"
#include "pajaro.h"

// -------------------------------------------------------
// clase gamewindow
// ventana principal del juego
// contiene:
//   - qtimer principal para cactus y colisiones
//   - qtimer para aparicion de pajaros
//   - qtimer para dificultad progresiva
//   - lista de cactus
//   - lista de objetos pajaro
// -------------------------------------------------------
class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    // eventos de dibujo y teclado
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // timer principal para mover cactus
    // y detectar colisiones
    void slot_update();

    // genera un nuevo pajaro
    void slot_spawnPajaro();

    // aumenta la dificultad con el tiempo
    void slot_increaseDifficulty();

    // repinta cuando un pajaro se mueve
    void slot_pajaroMoved();

    // reinicia la partida
    void slot_restart();

private:
    // ---- funciones de dibujo ----
    void drawBackground(QPainter &p);
    void drawGround(QPainter &p);
    void drawTRex(QPainter &p);
    void drawCactus(QPainter &p, int x, int variant);
    void drawPajaro(QPainter &p, const Pajaro *bird);
    void drawHUD(QPainter &p);
    void drawGameOver(QPainter &p);
    void drawCloud(QPainter &p, int cx, int cy, int w);

    // ---- funciones de logica ----
    void resetGame();
    void checkCollisions();
    bool rectsIntersect(const QRect &a, const QRect &b) const;

    // ---- entidades del juego ----
    TRex *m_trex;

    // lista de cactus con posicion x y variante
    QList<QPair<int,int>> m_cactus;

    // lista de pajaros activos
    QList<Pajaro*> m_pajaros;

    // ---- timers ----

    // timer principal del juego
    QTimer *m_mainTimer;

    // timer de aparicion de pajaros
    QTimer *m_birdSpawnTimer;

    // timer para aumentar dificultad
    QTimer *m_difficultyTimer;

    // ---- estado del juego ----
    bool  m_gameOver;
    bool  m_started;
    int   m_score;
    int   m_cactusSpeed;
    int   m_birdSpeed;
    int   m_cactusSpawnCounter;
    float m_bgScroll;

    // estructura para nubes decorativas
    struct Cloud {
        int x;
        int y;
        int w;
    };

    // lista de nubes
    QList<Cloud> m_clouds;
};

#endif // gamewindow_h