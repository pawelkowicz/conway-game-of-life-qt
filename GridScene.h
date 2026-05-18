#ifndef GRIDSCENE_H
#define GRIDSCENE_H

#include <QGraphicsScene>

class GridScene:public QGraphicsScene{
public:
    explicit GridScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    void drawBackground(QPainter *painter, const QRectF &rect) override;
};

#endif // GRIDSCENE_H
