#include "pantalla.h"
#include <QMessageBox>

Pantalla::Pantalla(QWidget* parent)
    : QWidget(parent)
{
}

void Pantalla::mostrarError(const QString& titulo, const QString& mensaje)
{
    QMessageBox::critical(this, titulo, mensaje);
}

void Pantalla::mostrarInfo(const QString& titulo, const QString& mensaje)
{
    QMessageBox::information(this, titulo, mensaje);
}
