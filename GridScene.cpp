#include "GridScene.h"
#include "qpainter.h"


void GridScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    int tileSize = 32;

    qreal left   = std::floor(rect.left() / tileSize) * tileSize;
    qreal right  = std::ceil(rect.right() / tileSize) * tileSize;
    qreal top    = std::floor(rect.top() / tileSize) * tileSize;
    qreal bottom = std::ceil(rect.bottom() / tileSize) * tileSize;

    painter->setPen(QPen(Qt::lightGray));

    // linie pionowe
    for(qreal x = left; x <= right; x += tileSize)
        painter->drawLine(QPointF(x, top), QPointF(x, bottom));

    // linie poziome
    for(qreal y = top; y <= bottom; y += tileSize)
        painter->drawLine(QPointF(left, y), QPointF(right, y));
}
