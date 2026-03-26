#pragma once
#include <QWidget>
#include "TrabajoPractico.h"

// Una fila de la grilla de TPs (QGridLayout la ubica en MainWidget)
class TPRowWidget : public QWidget {
    Q_OBJECT
public:
    explicit TPRowWidget(const TrabajoPractico& tp, QWidget* parent = nullptr);

signals:
    void editarRequested(const QString& id);
    void eliminarRequested(const QString& id);

private:
    QString m_id;
    static QString badgeStyle(const QString& color, const QString& bg);
};
