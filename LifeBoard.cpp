#include "LifeBoard.h"
#include <QGraphicsSceneMouseEvent>

LifeBoard::LifeBoard(int width, int height, int cellSize)
    : m_width(width), m_height(height), m_cellSize(cellSize)
{
    // Inicjalizacja pustej planszy
    int size = width * height;
    m_grid.resize(size);
    m_nextGrid.resize(size);
    m_grid.fill(false);
}

QRectF LifeBoard::boundingRect() const {
    return QRectF(0, 0, m_width * m_cellSize, m_height * m_cellSize);
}

void LifeBoard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Opcjonalnie: Rysowanie siatki (linii) - tylko jeśli potrzebne, bo zwalnia

    // painter->setPen(QPen(Qt::lightGray, 1));
    // for (int i = 0; i <= m_width; ++i)
    //     painter->drawLine(i * m_cellSize, 0, i * m_cellSize, m_height * m_cellSize);
    // for (int j = 0; j <= m_height; ++j)
    //     painter->drawLine(0, j * m_cellSize, m_width * m_cellSize, j * m_cellSize);


    // RYSOWANIE KOMÓREK
    painter->setBrush(QColor("#3498DB"));
    painter->setPen(Qt::NoPen); // Bez obramowania jest szybciej

    // Optymalizacja: Rysujemy serię prostokątów
    // W bardzo zaawansowanej wersji użylibyśmy QImage i setPixel,
    // ale drawRects jest wystarczająco szybkie dla setek tysięcy komórek.

    QVector<QRectF> rects;
    rects.reserve(1000); // Wstępna rezerwacja

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (m_grid[y * m_width + x]) {
                rects.append(QRectF(x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize));
            }
        }
    }

    if (!rects.isEmpty()) {
        painter->drawRects(rects);
    }
}

int LifeBoard::countNeighbors(int x, int y) const {
    int count = 0;
    // Sprawdzamy 8 sąsiadów
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int idx = index(x + dx, y + dy);
            if (idx != -1 && m_grid[idx]) {
                count++;
            }
        }
    }
    return count;
}

void LifeBoard::nextGeneration() {
    // Obliczanie logiki - czysta matematyka na tablicach, zero grafiki tutaj
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int idx = y * m_width + x;
            int neighbors = countNeighbors(x, y);
            bool isAlive = m_grid[idx];

            if (isAlive) {
                m_nextGrid[idx] = (neighbors == 2 || neighbors == 3);
            } else {
                m_nextGrid[idx] = (neighbors == 3);
            }
        }
    }

    // Zamiana tablic
    m_grid = m_nextGrid;

    // Wymuszenie odrysowania
    update();
}

void LifeBoard::toggleCell(QPointF pos) {
    int x = pos.x() / m_cellSize;
    int y = pos.y() / m_cellSize;
    int idx = index(x, y);

    if (idx != -1) {
        m_grid[idx] = !m_grid[idx];
        update();
    }
}

void LifeBoard::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        toggleCell(event->pos());
    }
}
