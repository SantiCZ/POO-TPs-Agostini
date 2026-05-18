#include "manager.h"
#include <QVBoxLayout>

Manager::Manager(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("T-Rex Extremo - Manager");
    setFixedSize(900, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_gameWindow = new GameWindow(this);
    layout->addWidget(m_gameWindow);
}

Manager::~Manager()
{
}
