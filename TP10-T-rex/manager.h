#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include "gamewindow.h"

// -------------------------------------------------------
// clase manager
// administra la ventana principal del juego
// actua como punto de entrada y controla
// el ciclo de vida de gamewindow
// -------------------------------------------------------
class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private:
    // ventana principal del juego
    GameWindow *m_gameWindow;
};

#endif // manager_h