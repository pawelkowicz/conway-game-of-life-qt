#ifndef GAME_H
#define GAME_H

#include "PatternPanel.h"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "Square.h"
#include "MainMenu.h"
#include "QTimer"
#include "qpushbutton.h"


class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget *parent = NULL);

    QGraphicsScene *scene;

    void addSquare(QPointF pos); //scenePos
    bool ifSquareHere(QPointF pos); //scenePos
    void removeSquareByPos(QPointF pos); //scenePos
    void removeSquare(QPointF pos); //scenePos

    QVector <Square *> aliveSquares;
    QVector <QPointF> aliveSquaresPos; // scenePos

    QPushButton *start;
    QPushButton *step;
    QPushButton *clear;
    QPushButton *speed;

    MainMenu *mainMenu;

    PatternPanel *pp;

    void startStop();
    void clearScene();
    void speedControl();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event);

private:
    QPoint lastMousePos;
    bool panning = false;
    QPoint toGridPos(const QPointF& scenePos);
    QPointF gridToScene(const QPoint& p);

    void move();

    bool running = false;
    QTimer *timer;
    int interval = 1000;
    int speedIndex = 0;
    QVector<int> speedIntervals = {1000, 500, 200, 100, 50, 10};


    void resizeEvent(QResizeEvent *event) override;

};


#endif // GAME_H
