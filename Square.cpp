#include "Square.h"
#include "qbrush.h"
#include "qpen.h"

Square::Square(QGraphicsRectItem *parent) {

    // setRect(0,0,32,32);
    // QBrush brush;
    // brush.setStyle(Qt::SolidPattern);
    // brush.setColor(QColor("#3498DB"));
    // setBrush(brush);
    // setPen(QPen(QColor("#216AA8"), 1));


    setRect(0, 0, 32, 32);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#50FA7B"));
    setBrush(brush);

    setPen(QPen(QColor("#1E7F4F"), 1));

}
