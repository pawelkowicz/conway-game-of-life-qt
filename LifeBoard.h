#ifndef LIFEBOARD_H
#define LIFEBOARD_H

// LifeBoard.h
#include <QGraphicsItem>
#include <QVector>
#include <QPainter>

class LifeBoard : public QGraphicsItem {
public:
    LifeBoard(int width, int height, int cellSize);

    // Wymagane przez QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Logika gry
    void nextGeneration();
    void toggleCell(QPointF pos); // Kliknięcie myszką

protected:
    // Obsługa kliknięć bezpośrednio w tym itemie
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_width;    // Liczba kolumn
    int m_height;   // Liczba wierszy
    int m_cellSize; // Rozmiar kratki w pikselach

    // Dwie tablice dla podwójnego buforowania
    QVector<bool> m_grid;
    QVector<bool> m_nextGrid;

    // Funkcja pomocnicza do indeksowania tablicy 1D jak 2D
    inline int index(int x, int y) const {
        // Obsługa zawijania (torus) lub warunki brzegowe
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return -1;
        return y * m_width + x;
    }

    int countNeighbors(int x, int y) const;
};

#endif // LIFEBOARD_H
