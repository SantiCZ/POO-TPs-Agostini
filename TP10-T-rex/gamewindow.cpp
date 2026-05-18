// ---- dimensiones de la ventana ----
static const int WIN_W     = 900;
static const int WIN_H     = 400;
static const int GROUND_Y  = 330;

// ---- dimensiones de los cactus ----
static const int CACTUS_W  = 30;
static const int CACTUS_H  = 60;

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    m_gameOver(false),
    m_started(false),
    m_score(0),
    m_cactusSpeed(6),
    m_birdSpeed(5),
    m_cactusSpawnCounter(0),
    m_bgScroll(0.f)
{
    // configura el tamano fijo de la ventana
    setFixedSize(WIN_W, WIN_H);

    // permite recibir eventos de teclado
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // crea el dinosaurio principal
    m_trex = new TRex(this);

    // crea nubes iniciales aleatorias
    for (int i = 0; i < 4; ++i) {
        Cloud c;
        c.x = QRandomGenerator::global()->bounded(100, WIN_W);
        c.y = QRandomGenerator::global()->bounded(40, 160);
        c.w = QRandomGenerator::global()->bounded(60, 130);
        m_clouds.append(c);
    }

    // timer principal del juego
    // controla cactus colisiones y fondo
    m_mainTimer = new QTimer(this);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(slot_update()));
    m_mainTimer->start(16);

    // timer que genera pajaros cada 5 segundos
    m_birdSpawnTimer = new QTimer(this);
    connect(m_birdSpawnTimer, SIGNAL(timeout()), this, SLOT(slot_spawnPajaro()));
    m_birdSpawnTimer->start(5000);

    // timer que aumenta la dificultad
    m_difficultyTimer = new QTimer(this);
    connect(m_difficultyTimer, SIGNAL(timeout()), this, SLOT(slot_increaseDifficulty()));
    m_difficultyTimer->start(3000);
}

GameWindow::~GameWindow()
{
    // elimina todos los pajaros restantes
    qDeleteAll(m_pajaros);
}

// ============================================================
// slot del timer principal
// ============================================================
void GameWindow::slot_update()
{
    // si el juego no comenzo o termino solo repinta
    if (m_gameOver || !m_started) {
        update();
        return;
    }

    // aumenta puntaje y scroll del fondo
    m_score++;
    m_bgScroll += 1.5f;

    // mueve las nubes decorativas
    for (auto &c : m_clouds) {
        c.x -= 1;

        // recicla nubes cuando salen de pantalla
        if (c.x + c.w < 0) {
            c.x = WIN_W + 10;
            c.y = QRandomGenerator::global()->bounded(40, 160);
            c.w = QRandomGenerator::global()->bounded(60, 130);
        }
    }

    // mueve todos los cactus
    for (auto &cac : m_cactus) {
        cac.first -= m_cactusSpeed;
    }

    // elimina cactus fuera de pantalla
    while (!m_cactus.isEmpty() && m_cactus.first().first + CACTUS_W < 0) {
        m_cactus.removeFirst();
    }

    // contador para generar nuevos cactus
    m_cactusSpawnCounter++;

    // reduce el intervalo a medida que aumenta el puntaje
    int spawnInterval = qMax(40, 90 - m_score / 100);

    // crea un nuevo cactus aleatorio
    if (m_cactusSpawnCounter >= spawnInterval) {
        int variant = QRandomGenerator::global()->bounded(3);
        m_cactus.append(qMakePair(WIN_W + 20, variant));
        m_cactusSpawnCounter = 0;
    }

    // elimina pajaros que salieron de pantalla
    for (int i = m_pajaros.size() - 1; i >= 0; --i) {
        if (m_pajaros[i]->isOffScreen()) {
            delete m_pajaros.takeAt(i);
        }
    }

    // verifica colisiones
    checkCollisions();

    // solicita repintado
    update();
}

// ============================================================
// genera un nuevo pajaro
// ============================================================
void GameWindow::slot_spawnPajaro()
{
    // no generar si el juego termino
    if (m_gameOver || !m_started)
        return;

    // elige un tipo aleatorio de pajaro
    int tipoInt = QRandomGenerator::global()->bounded(3);

    Pajaro::TipoPajaro tipo =
        static_cast<Pajaro::TipoPajaro>(tipoInt);

    // crea el pajaro
    Pajaro *bird = new Pajaro(WIN_W + 20,
                              WIN_W,
                              tipo,
                              this);

    // aplica velocidad actual
    bird->setSpeed(m_birdSpeed);

    // conecta senal de movimiento
    connect(bird,
            SIGNAL(moved()),
            this,
            SLOT(slot_pajaroMoved()));

    // agrega el pajaro a la lista
    m_pajaros.append(bird);

    // genera un pajaro sorpresa adicional
    QTimer::singleShot(2500,
                       this,
                       SLOT(slot_spawnPajaro()));
}

// ============================================================
// aumenta la dificultad progresivamente
// ============================================================
void GameWindow::slot_increaseDifficulty()
{
    // no hacer nada si el juego termino
    if (m_gameOver || !m_started)
        return;

    // aumenta velocidades con limite maximo
    if (m_cactusSpeed < 18)
        m_cactusSpeed++;

    if (m_birdSpeed < 16)
        m_birdSpeed++;

    // actualiza velocidad de los pajaros existentes
    for (Pajaro *bird : m_pajaros) {
        bird->setSpeed(m_birdSpeed);
    }
}

// ============================================================
// repinta cuando un pajaro se mueve
// ============================================================
void GameWindow::slot_pajaroMoved()
{
    update();
}

// ============================================================
// reinicia la partida
// ============================================================
void GameWindow::slot_restart()
{
    resetGame();
}

// ============================================================
// reinicia completamente el juego
// ============================================================
void GameWindow::resetGame()
{
    m_gameOver           = false;
    m_started            = true;
    m_score              = 0;
    m_cactusSpeed        = 6;
    m_birdSpeed          = 5;
    m_cactusSpawnCounter = 0;
    m_bgScroll           = 0.f;

    // elimina todos los cactus
    m_cactus.clear();

    // elimina todos los pajaros
    qDeleteAll(m_pajaros);
    m_pajaros.clear();

    // reinicia el dinosaurio
    delete m_trex;
    m_trex = new TRex(this);

    // reinicia timers
    m_mainTimer->start(16);
    m_birdSpawnTimer->start(5000);
    m_difficultyTimer->start(3000);

    // devuelve el foco a la ventana
    setFocus();

    update();
}