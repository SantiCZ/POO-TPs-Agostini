#ifndef PANELCURRICULUM_H
#define PANELCURRICULUM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>

class PanelCurriculum : public QWidget
{
    Q_OBJECT

public:
    explicit PanelCurriculum(QWidget *parent = nullptr);

private:
    void construirPanel();
    QVBoxLayout *m_layout;
};

#endif // PANELCURRICULUM_H