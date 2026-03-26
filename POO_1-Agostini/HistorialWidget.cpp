#include "HistorialWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidgetItem>
#include <QFrame>

HistorialWidget::HistorialWidget(HistorialRepository* histRepo, QWidget* parent)
    : QWidget(parent), m_histRepo(histRepo) {
    setWindowTitle("Historial de acciones");
    setMinimumSize(560, 460);
    buildUI();
}

void HistorialWidget::buildUI() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(20, 16, 20, 16);
    root->setSpacing(10);

    // Cabecera
    auto* topBar = new QHBoxLayout;
    auto* title  = new QLabel("Historial de acciones");
    title->setStyleSheet("font-size:18px; font-weight:bold;");
    auto* btnVolver = new QPushButton("← Volver");
    topBar->addWidget(title);
    topBar->addStretch();
    topBar->addWidget(btnVolver);
    root->addLayout(topBar);

    // Cabecera de columnas
    auto* headerRow = new QHBoxLayout;
    auto mkHdr = [](const QString& txt, int stretch) {
        auto* l = new QLabel(txt);
        l->setStyleSheet("font-size:11px; color:#888; font-weight:bold; "
                         "text-transform:uppercase; letter-spacing:1px;");
        return qMakePair(l, stretch);
    };
    auto [lFecha,  s1] = mkHdr("Fecha y hora", 2);
    auto [lAccion, s2] = mkHdr("Acción", 1);
    auto [lDetalle,s3] = mkHdr("Detalle", 3);
    headerRow->addWidget(lFecha,   s1);
    headerRow->addWidget(lAccion,  s2);
    headerRow->addWidget(lDetalle, s3);
    root->addLayout(headerRow);

    auto* sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#e0e0e0;");
    root->addWidget(sep);

    // Lista
    m_lista = new QListWidget;
    m_lista->setSpacing(2);
    m_lista->setAlternatingRowColors(true);
    m_lista->setStyleSheet(
        "QListWidget { border:none; font-size:13px; }"
        "QListWidget::item { padding:6px 4px; }"
        "QListWidget::item:alternate { background:#f9f9f9; }"
    );
    root->addWidget(m_lista, 1);

    connect(btnVolver, &QPushButton::clicked, this, &HistorialWidget::cerrado);
}

void HistorialWidget::cargar(const QString& usuario) {
    m_lista->clear();
    QList<AccionHistorial> acciones = m_histRepo->porUsuario(usuario);

    if (acciones.isEmpty()) {
        auto* item = new QListWidgetItem("Sin acciones registradas.");
        item->setForeground(Qt::gray);
        m_lista->addItem(item);
        return;
    }

    for (const AccionHistorial& a : acciones) {
        QString linea = QString("%1    [%2]    %3")
                        .arg(a.timestamp.toString("dd/MM/yyyy HH:mm:ss"), -22)
                        .arg(a.accion, -16)
                        .arg(a.detalle);
        auto* item = new QListWidgetItem(linea);
        item->setToolTip(a.detalle);

        // Colorear según tipo de acción
        if (a.accion.contains("Eliminar"))
            item->setForeground(QColor("#c0392b"));
        else if (a.accion.contains("Crear"))
            item->setForeground(QColor("#1d6a39"));
        else if (a.accion.contains("notas"))
            item->setForeground(QColor("#1565c0"));

        m_lista->addItem(item);
    }
}
