#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>
#include <QGroupBox>
#include <QDoubleSpinBox>

#include "servermonitor.h"
#include "metriccardwidget.h"
#include "eventlogwidget.h"
#include "statusbadge.h"

// ──────────────────────────────────────────────────────────
//  MainWindow  (basado en QWidget, sin QMainWindow)
//
//  Widgets interactivos y su justificación:
//  - QLineEdit      → ingresar URL del endpoint (texto libre)
//  - QSpinBox       → intervalo de chequeo en segundos (entero acotado)
//  - QDoubleSpinBox → umbral CPU/RAM para alertas (decimal fino)
//  - QPushButton    → refresco manual, iniciar/detener, limpiar log
//  - QLabel         → último chequeo, uptime, countdown (lectura rápida)
//  - StatusBadge    → indicador visual del estado general
//  - MetricCardWidget → tarjetas de métricas (CPU, RAM, Disco, Red)
//  - EventLogWidget   → historial de eventos con scroll
// ──────────────────────────────────────────────────────────
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMetricsUpdated(const ServerMetrics &m);
    void onConnectionError(const QString &err);
    void onStatusChanged(const QString &newStatus, const QString &oldStatus);
    void onNewEventLogged(const EventEntry &e);

    void onRefreshClicked();
    void onStartStopClicked();
    void onClearLogClicked();
    void onIntervalChanged(int secs);
    void onThresholdCpuChanged(double pct);
    void onThresholdMemChanged(double pct);

    void updateCountdownLabel();

private:
    void setupUi();
    void setupTopBar(QWidget *container);
    void setupConfigPanel(QWidget *container);
    void setupMetricsGrid(QWidget *container);
    void setupBottomPanel(QWidget *container);
    void applyDarkTheme();

    // Barra de estado propia (reemplaza QStatusBar)
    void setStatusMessage(const QString &msg);

    // ── Núcleo ──────────────────────────────
    ServerMonitor *m_monitor = nullptr;

    // ── Controles de configuración ──────────
    QLineEdit      *m_urlEdit         = nullptr;
    QSpinBox       *m_intervalSpin    = nullptr;
    QDoubleSpinBox *m_threshCpuSpin   = nullptr;
    QDoubleSpinBox *m_threshMemSpin   = nullptr;
    QPushButton    *m_startStopBtn    = nullptr;
    QPushButton    *m_refreshBtn      = nullptr;
    QPushButton    *m_clearLogBtn     = nullptr;

    // ── Estado ──────────────────────────────
    StatusBadge    *m_statusBadge     = nullptr;
    QLabel         *m_lastCheckLabel  = nullptr;
    QLabel         *m_uptimeLabel     = nullptr;
    QLabel         *m_countdownLabel  = nullptr;
    QLabel         *m_statusBarLabel  = nullptr;   // reemplaza QStatusBar
    QTimer         *m_countdownTimer  = nullptr;
    int             m_secondsToNext   = 0;

    // ── Métricas ────────────────────────────
    MetricCardWidget *m_cardCpu       = nullptr;
    MetricCardWidget *m_cardMem       = nullptr;
    MetricCardWidget *m_cardDisk      = nullptr;
    MetricCardWidget *m_cardRx        = nullptr;
    MetricCardWidget *m_cardTx        = nullptr;

    // ── Log ─────────────────────────────────
    EventLogWidget *m_eventLog        = nullptr;
};

#endif // MAINWINDOW_H