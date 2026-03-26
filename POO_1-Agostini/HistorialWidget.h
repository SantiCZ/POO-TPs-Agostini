#pragma once
#include <QWidget>
#include <QListWidget>
#include "HistorialRepository.h"

class HistorialWidget : public QWidget {
    Q_OBJECT
public:
    explicit HistorialWidget(HistorialRepository* histRepo, QWidget* parent = nullptr);

    void cargar(const QString& usuario);

signals:
    void cerrado();

private:
    HistorialRepository* m_histRepo;
    QListWidget*         m_lista;
    void buildUI();
};
