#include "TPRowWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

TPRowWidget::TPRowWidget(const TrabajoPractico& tp, QWidget* parent)
    : QWidget(parent), m_id(tp.id) {

    auto* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet(
        "QFrame { background:#ffffff; border:1px solid #e0e0e0;"
        "border-radius:6px; padding:6px; }"
    );

    // --- Info ---
    auto* infoLay = new QVBoxLayout;
    infoLay->setSpacing(3);

    auto* topRow = new QHBoxLayout;
    topRow->setSpacing(6);

    auto* lblNombre = new QLabel(tp.nombre);
    lblNombre->setStyleSheet("font-size:14px; font-weight:bold;");
    topRow->addWidget(lblNombre);

    // Badge prioridad
    QString pc, pb;
    switch (tp.prioridad) {
        case PrioridadTP::Alta:  pc="#7b241c"; pb="#fadbd8"; break;
        case PrioridadTP::Media: pc="#784212"; pb="#fdebd0"; break;
        case PrioridadTP::Baja:  pc="#1d6a39"; pb="#d5f5e3"; break;
    }
    auto* lblPrio = new QLabel(TrabajoPractico::prioridadToString(tp.prioridad));
    lblPrio->setStyleSheet(badgeStyle(pc, pb));
    topRow->addWidget(lblPrio);

    // Badge estado
    QString ec, eb;
    switch (tp.estado) {
        case EstadoTP::Pendiente:  ec="#616161"; eb="#f5f5f5"; break;
        case EstadoTP::EnCurso:    ec="#1565c0"; eb="#e3f2fd"; break;
        case EstadoTP::Entregado:  ec="#1d6a39"; eb="#d5f5e3"; break;
        case EstadoTP::Vencido:    ec="#7b241c"; eb="#fadbd8"; break;
    }
    auto* lblEstado = new QLabel(TrabajoPractico::estadoToString(tp.estado));
    lblEstado->setStyleSheet(badgeStyle(ec, eb));
    topRow->addWidget(lblEstado);
    topRow->addStretch();
    infoLay->addLayout(topRow);

    if (!tp.descripcion.isEmpty()) {
        auto* lbl = new QLabel(tp.descripcion);
        lbl->setStyleSheet("color:#555; font-size:12px;");
        lbl->setWordWrap(true);
        infoLay->addWidget(lbl);
    }

    if (tp.fechaEntrega.isValid()) {
        auto* lbl = new QLabel("Entrega: " + tp.fechaEntrega.toString("dd/MM/yyyy"));
        lbl->setStyleSheet("color:#888; font-size:11px;");
        infoLay->addWidget(lbl);
    }

    if (!tp.notas.isEmpty()) {
        auto* lbl = new QLabel("Tiene notas guardadas");
        lbl->setStyleSheet("color:#aaa; font-size:11px; font-style:italic;");
        infoLay->addWidget(lbl);
    }

    // --- Botones ---
    auto* btnLay = new QVBoxLayout;
    btnLay->setSpacing(4);
    btnLay->setAlignment(Qt::AlignTop | Qt::AlignRight);

    auto* btnEdit = new QPushButton("Editar");
    auto* btnDel  = new QPushButton("Eliminar");
    btnEdit->setFixedWidth(75);
    btnDel->setFixedWidth(75);
    btnDel->setStyleSheet(
        "QPushButton{color:#c0392b; border:1px solid #c0392b; border-radius:4px; padding:4px 8px;}"
        "QPushButton:hover{background:#fadbd8;}"
    );
    btnLay->addWidget(btnEdit);
    btnLay->addWidget(btnDel);
    btnLay->addStretch();

    // Frame layout
    auto* frameLay = new QHBoxLayout(frame);
    frameLay->addLayout(infoLay, 1);
    frameLay->addLayout(btnLay);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->addWidget(frame);

    connect(btnEdit, &QPushButton::clicked, this, [this]{ emit editarRequested(m_id); });
    connect(btnDel,  &QPushButton::clicked, this, [this]{ emit eliminarRequested(m_id); });
}

QString TPRowWidget::badgeStyle(const QString& color, const QString& bg) {
    return QString("background:%1; color:%2; border-radius:10px;"
                   "padding:2px 8px; font-size:11px; font-weight:bold;").arg(bg, color);
}
