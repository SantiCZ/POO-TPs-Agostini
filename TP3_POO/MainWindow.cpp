#include "MainWindow.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QLineEdit>

// ── Color palette helper ──────────────────────────────────────────────────────

QColor MainWindow::colorForIndex(int idx) const {
    float t = (idx <= 0) ? 0.f : (idx >= 8) ? 1.f : idx / 8.f;
    int r = qRound(192 + (24  - 192) * t);
    int g = qRound(19  + (233 - 19)  * t);
    int b = qRound(76  + (199 - 76)  * t);
    return QColor(r, g, b);
}

// ── Constructor ───────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Collaborative Canvas");
    resize(1200, 800);

    m_model  = new DrawingModel(this);
    m_canvas = new CanvasView(m_model, this);
    m_sync   = new SyncManager(m_model, this);

    // Cargar y limpiar URL del servidor
    QSettings settings("CollabCanvas", "CollabCanvas");
    QString savedUrl = settings.value("serverUrl", "http://161.97.92.143:3001").toString();
    QUrl parsedUrl(savedUrl);
    QString serverUrl = parsedUrl.scheme() + "://" + parsedUrl.host()
                        + ":" + QString::number(parsedUrl.port(3001));

    bool ok;
    serverUrl = QInputDialog::getText(
        this, "Server URL",
        "Ingresá la URL del servidor VPS (ej: http://161.97.92.143:3001):",
        QLineEdit::Normal, serverUrl, &ok);
    if (ok && !serverUrl.isEmpty()) {
        while (serverUrl.endsWith('/')) serverUrl.chop(1);
        settings.setValue("serverUrl", serverUrl);
    }
    m_sync->setServerUrl(serverUrl);

    // Layout principal: toolbar arriba, canvas en el medio, statusbar abajo
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(buildToolbar());
    mainLayout->addWidget(m_canvas, 1);   // 1 = ocupa todo el espacio restante
    mainLayout->addWidget(buildStatusBar());

    applyMetroStyle();

    // Connections
    connect(m_canvas, &CanvasView::thicknessChanged,
            this, &MainWindow::onThicknessChanged);
    connect(m_canvas, &CanvasView::strokeFinished, this, [this]() {
        m_sync->saveToServer();
    });
    connect(m_sync, &SyncManager::statusMessage, this, [this](const QString &msg) {
        m_statusLabel->setText(msg);
    });
    connect(m_sync, &SyncManager::saveSuccess,  this, &MainWindow::onSaveSuccess);
    connect(m_sync, &SyncManager::saveError,    this, &MainWindow::onSaveError);
    connect(m_sync, &SyncManager::fetchSuccess, this, [this]() {
        m_statusLabel->setText("✓ Sincronizado");
    });

    // Fetch inicial + polling
    m_sync->fetchFromServer();
    m_sync->startPolling(5000);

    // Color inicial
    m_canvas->setColor(colorForIndex(0));
    updateColorSwatch();
}

MainWindow::~MainWindow() {}

// ── Toolbar ───────────────────────────────────────────────────────────────────

QWidget* MainWindow::buildToolbar() {
    QWidget *bar = new QWidget();
    bar->setObjectName("toolbar");
    bar->setFixedHeight(48);

    QHBoxLayout *layout = new QHBoxLayout(bar);
    layout->setContentsMargins(8, 4, 8, 4);
    layout->setSpacing(8);

    // Botón Guardar (estilo Metro)
    m_saveButton = new QPushButton("  💾  GUARDAR");
    m_saveButton->setObjectName("metroSaveButton");
    m_saveButton->setFixedHeight(36);
    m_saveButton->setMinimumWidth(140);
    connect(m_saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    layout->addWidget(m_saveButton);

    // Separador
    auto sep = [&]() {
        QFrame *f = new QFrame();
        f->setFrameShape(QFrame::VLine);
        f->setObjectName("separator");
        f->setFixedWidth(1);
        layout->addWidget(f);
    };

    sep();

    // Color swatch
    QLabel *colorLabel = new QLabel("Color:");
    colorLabel->setObjectName("toolLabel");
    layout->addWidget(colorLabel);

    m_colorSwatch = new QLabel();
    m_colorSwatch->setFixedSize(28, 28);
    m_colorSwatch->setObjectName("colorSwatch");
    layout->addWidget(m_colorSwatch);

    QLabel *keysHint = new QLabel("Teclas 1–9");
    keysHint->setObjectName("hintLabel");
    layout->addWidget(keysHint);

    sep();

    // Grosor
    QLabel *thickLabel = new QLabel("Grosor:");
    thickLabel->setObjectName("toolLabel");
    layout->addWidget(thickLabel);

    m_thicknessLabel = new QLabel("6 px");
    m_thicknessLabel->setObjectName("thicknessLabel");
    m_thicknessLabel->setMinimumWidth(50);
    layout->addWidget(m_thicknessLabel);

    QLabel *scrollHint = new QLabel("(scroll)");
    scrollHint->setObjectName("hintLabel");
    layout->addWidget(scrollHint);

    sep();

    // Paleta de colores
    QLabel *paletteLabel = new QLabel("Paleta:");
    paletteLabel->setObjectName("toolLabel");
    layout->addWidget(paletteLabel);

    for (int i = 0; i < 9; ++i) {
        QLabel *swatch = new QLabel(QString::number(i + 1));
        swatch->setFixedSize(22, 22);
        swatch->setAlignment(Qt::AlignCenter);
        QColor c = colorForIndex(i);
        swatch->setStyleSheet(QString(
                                  "background:%1; color:%2; font-size:10px; font-weight:bold; border:1px solid #555;")
                                  .arg(c.name())
                                  .arg(c.lightness() > 128 ? "#000" : "#fff"));
        layout->addWidget(swatch);
    }

    sep();

    QLabel *hint = new QLabel("Click izq: Dibujar  |  Click der: Borrar");
    hint->setObjectName("hintLabel");
    layout->addWidget(hint);

    layout->addStretch();   // empuja todo a la izquierda

    return bar;
}

// ── Status bar ────────────────────────────────────────────────────────────────

QWidget* MainWindow::buildStatusBar() {
    QWidget *bar = new QWidget();
    bar->setObjectName("statusBar");
    bar->setFixedHeight(24);

    QHBoxLayout *layout = new QHBoxLayout(bar);
    layout->setContentsMargins(8, 0, 8, 0);

    m_statusLabel = new QLabel("Listo");
    m_statusLabel->setObjectName("statusLabel");
    layout->addWidget(m_statusLabel);
    layout->addStretch();

    return bar;
}

// ── Styling ───────────────────────────────────────────────────────────────────

void MainWindow::applyMetroStyle() {
    setStyleSheet(R"(
        QWidget {
            background: #1a1a2e;
            color: #ffffff;
        }

        QWidget#toolbar {
            background: #16213e;
            border-bottom: 2px solid #0f3460;
        }

        QWidget#statusBar {
            background: #16213e;
            border-top: 1px solid #0f3460;
        }

        QFrame#separator {
            background: #0f3460;
            margin: 6px 2px;
        }

        QPushButton#metroSaveButton {
            background: #c0134c;
            color: #ffffff;
            border: none;
            font-family: 'Segoe UI', 'Arial', sans-serif;
            font-size: 12px;
            font-weight: bold;
            letter-spacing: 2px;
            padding: 0 16px;
            text-transform: uppercase;
        }
        QPushButton#metroSaveButton:hover {
            background: #e01558;
        }
        QPushButton#metroSaveButton:pressed {
            background: #960f3a;
        }
        QPushButton#metroSaveButton:disabled {
            background: #555555;
            color: #999999;
        }

        QLabel#toolLabel {
            color: #8899bb;
            font-size: 11px;
            font-family: 'Segoe UI', sans-serif;
            padding: 0 2px;
            background: transparent;
        }
        QLabel#hintLabel {
            color: #556688;
            font-size: 10px;
            font-family: 'Segoe UI', sans-serif;
            padding: 0 2px;
            background: transparent;
        }
        QLabel#thicknessLabel {
            color: #18e9c7;
            font-size: 12px;
            font-weight: bold;
            font-family: 'Consolas', 'Courier New', monospace;
            background: transparent;
        }
        QLabel#statusLabel {
            color: #8899bb;
            font-size: 11px;
            font-family: 'Segoe UI', sans-serif;
            background: transparent;
        }
    )");
}

// ── Color swatch ──────────────────────────────────────────────────────────────

void MainWindow::updateColorSwatch() {
    QColor c = colorForIndex(m_colorIndex);
    m_colorSwatch->setStyleSheet(QString(
                                     "background:%1; border:2px solid %2; border-radius:3px;")
                                     .arg(c.name())
                                     .arg(c.darker(150).name()));
}

// ── Key events ────────────────────────────────────────────────────────────────

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key >= Qt::Key_1 && key <= Qt::Key_9) {
        m_colorIndex = key - Qt::Key_1;
        QColor c = colorForIndex(m_colorIndex);
        m_canvas->setColor(c);
        updateColorSwatch();
        m_statusLabel->setText(QString("Color %1: %2").arg(m_colorIndex + 1).arg(c.name()));
    }
    QWidget::keyPressEvent(event);
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void MainWindow::onSaveClicked() {
    m_saveButton->setEnabled(false);
    m_saveButton->setText("  ⏳  GUARDANDO…");
    m_sync->saveToServer();
}

void MainWindow::onThicknessChanged(int t) {
    m_thicknessLabel->setText(QString("%1 px").arg(t));
}

void MainWindow::onSaveSuccess() {
    if (!m_saveButton->isEnabled()) {
        m_saveButton->setEnabled(true);
        m_saveButton->setText("  💾  GUARDAR");
    }
    m_statusLabel->setText("✓ Sincronizado");
}

void MainWindow::onSaveError(const QString &msg) {
    if (!m_saveButton->isEnabled()) {
        m_saveButton->setEnabled(true);
        m_saveButton->setText("  💾  GUARDAR");
        QMessageBox::warning(this, "Error al guardar", msg);
    } else {
        m_statusLabel->setText("⚠ Error al sincronizar: " + msg);
    }
}