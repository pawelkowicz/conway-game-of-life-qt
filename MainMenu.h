#ifndef MAINMENU_H
#define MAINMENU_H

#include "qpushbutton.h"
#include <QObject>
#include <QWidget>

class Game;

class MainMenu :public QWidget
{
    Q_OBJECT
public:
    MainMenu(Game *game, QWidget *parent);

    QPushButton *startButton;

    void start();

    Game *m_game;
};

#endif // MAINMENU_H
