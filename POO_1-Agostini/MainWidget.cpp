#include "MainWidget.h"
#include "TPRowWidget.h"
#include "TrabajoPractico.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFrame>

MainWidget::MainWidget(TPRepository*       tpRepo,
                       HistorialRepository* histRepo,
                       SessionManager*     session,
                       QWidget*            parent)
    : QWidget(parent)
    , m_tpRepo(tpRepo)
    , m_histRepo(histRepo)
    , m_session(session)
{
    setWindowTitle("Planificador de TPs");
    setMinimumSize(700, 500);
    buildUI();
}

void MainWidget::buildUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(10);

    // ── Barra superior ──────────────────────────────────────────────────────
    auto* topBar = new QHBoxLayout;

    m_lblBienvenida = new QLabel("Mis TPs");
    m_lblBienvenida->setStyleSheet("font-size:18px; font-weight:bold;");

    m_lblTimer = new QLabel;
    m_lblTimer->setStyleSheet("color:#888; font-size:12px;");

    auto* btnNuevo    = new QPushButton("+ Nuevo TP");
    auto* btnHistorial= new QPushButton("Historial");
    auto* btnLogout   = new QPushButton("Cerrar sesión");

    btnNuevo->setStyleSheet(
        "QPushButton{background:#1565c0;color:white;border:none;"
        "border-radius:4px;padding:6px 14px;font-weight:bold;}"
        "QPushButton:hover{background:#0d47a1;}"
    );

    topBar->addWidget(m_lblBienvenida);
    topBar->addWidget(m_lblTimer);
    topBar->addStretch();
    topBar->addWidget(btnNuevo);
    topBar->addWidget(btnHistorial);
    topBar->addWidget(btnLogout);
    root->addLayout(topBar);

    // ── Barra de filtros ────────────────────────────────────────────────────
    auto* filterBar = new QHBoxLayout;
    filterBar->setSpacing(8);

    m_cmbEstado = new QComboBox;
    m_cmbEstado->addItem("Todos los estados", "");
    for (const QString& e : TrabajoPractico::estadosList())
        m_cmbEstado->addItem(e, e);

    m_cmbPrioridad = new QComboBox;
    m_cmbPrioridad->addItem("Todas las prioridades", "");
    for (const QString& p : TrabajoPractico::prioridadesList())
        m_cmbPrioridad->addItem(p, p);

    m_txtBuscar = new QLineEdit;
    m_txtBuscar->setPlaceholderText("Buscar por nombre o descripción...");

    filterBar->addWidget(m_cmbEstado);
    filterBar->addWidget(m_cmbPrioridad);
    filterBar->addWidget(m_txtBuscar, 1);
    root->addLayout(filterBar);

    // ── Grilla con scroll ───────────────────────────────────────────────────
    m_gridContainer = new QWidget;
    m_gridContainer->setStyleSheet("background: transparent;");
    m_grid = new QGridLayout(m_gridContainer);
    m_grid->setContentsMargins(0, 0, 0, 0);
    m_grid->setVerticalSpacing(6);
    m_grid->setColumnStretch(0, 1); // columna única estirable

    m_scroll = new QScrollArea;
    m_scroll->setWidget(m_gridContainer);
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    root->addWidget(m_scroll, 1);

    m_lblVacio = new QLabel("No hay trabajos prácticos. Creá uno con \"+ Nuevo TP\".");
    m_lblVacio->setAlignment(Qt::AlignCenter);
    m_lblVacio->setStyleSheet("color:#aaa; font-size:14px; margin:32px;");
    m_lblVacio->hide();
    root->addWidget(m_lblVacio);

    // ── Conexiones ──────────────────────────────────────────────────────────
    connect(btnNuevo,     &QPushButton::clicked, this, &MainWidget::onNuevo);
    connect(btnHistorial, &QPushButton::clicked, this, &MainWidget::onHistorial);
    connect(btnLogout,    &QPushButton::clicked, this, &MainWidget::onLogout);
    connect(m_cmbEstado,    QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWidget::onFiltroChanged);
    connect(m_cmbPrioridad, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWidget::onFiltroChanged);
    connect(m_txtBuscar,  &QLineEdit::textChanged, this, &MainWidget::onFiltroChanged);
}

void MainWidget::setUsuario(const QString& username) {
    m_usuario = username;
    m_lblBienvenida->setText("Mis TPs — " + username);
    refrescarGrilla();
}

void MainWidget::refrescarGrilla() {
    limpiarGrilla();

    QString filtroEstado    = m_cmbEstado->currentData().toString();
    QString filtroPrioridad = m_cmbPrioridad->currentData().toString();
    QString filtroBuscar    = m_txtBuscar->text().trimmed().toLower();

    QList<TrabajoPractico> tps = m_tpRepo->porUsuario(m_usuario);

    // Filtrar
    QList<TrabajoPractico> filtrados;
    for (const TrabajoPractico& tp : tps) {
        if (!filtroEstado.isEmpty() &&
            TrabajoPractico::estadoToString(tp.estado) != filtroEstado) continue;
        if (!filtroPrioridad.isEmpty() &&
            TrabajoPractico::prioridadToString(tp.prioridad) != filtroPrioridad) continue;
        if (!filtroBuscar.isEmpty() &&
            !tp.nombre.toLower().contains(filtroBuscar) &&
            !tp.descripcion.toLower().contains(filtroBuscar)) continue;
        filtrados.append(tp);
    }

    if (filtrados.isEmpty()) {
        m_scroll->hide();
        m_lblVacio->show();
        return;
    }
    m_lblVacio->hide();
    m_scroll->show();

    // Llenar grilla: 1 TP por fila, columna 0
    for (int i = 0; i < filtrados.size(); ++i) {
        auto* row = new TPRowWidget(filtrados[i], m_gridContainer);
        m_grid->addWidget(row, i, 0);
        connect(row, &TPRowWidget::editarRequested,   this, &MainWidget::onEditarTP);
        connect(row, &TPRowWidget::eliminarRequested, this, &MainWidget::onEliminarTP);
    }
    // Spacer para empujar filas hacia arriba
    m_grid->setRowStretch(filtrados.size(), 1);
}

void MainWidget::limpiarGrilla() {
    QLayoutItem* item;
    while ((item = m_grid->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

void MainWidget::onSessionTick(int remaining) {
    int m = remaining / 60;
    int s = remaining % 60;
    m_lblTimer->setText(QString("Sesión expira en %1:%2")
                        .arg(m).arg(s, 2, 10, QChar('0')));
}

void MainWidget::onNuevo()     { emit nuevoTPRequested(); }
void MainWidget::onLogout()    { emit logoutRequested(); }
void MainWidget::onHistorial() { emit historialRequested(); }
void MainWidget::onFiltroChanged() { refrescarGrilla(); }

void MainWidget::onEditarTP(const QString& id) {
    emit editarTPRequested(id);
}

void MainWidget::onEliminarTP(const QString& id) {
    TrabajoPractico* tp = m_tpRepo->buscarPorId(id);
    QString nombre = tp ? tp->nombre : id;

    QMessageBox::StandardButton resp = QMessageBox::question(
        this, "Eliminar TP",
        QString("¿Eliminar \"%1\"?\nEsta acción no se puede deshacer.").arg(nombre),
        QMessageBox::Yes | QMessageBox::No
    );
    if (resp != QMessageBox::Yes) return;

    m_tpRepo->eliminar(id);
    m_histRepo->registrar(m_usuario, "Eliminar TP",
                          QString("Se eliminó \"%1\"").arg(nombre));
    refrescarGrilla();
}
