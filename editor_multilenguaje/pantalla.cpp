#include "pantalla.h"
#include <QResizeEvent>

Pantalla::Pantalla(QWidget *parent)
    : QWidget(parent)
{
}

Pantalla::~Pantalla()
{
}

void Pantalla::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    registrarEvento(QString("resizeEvent: %1x%2")
                        .arg(event->size().width())
                        .arg(event->size().height()));
}