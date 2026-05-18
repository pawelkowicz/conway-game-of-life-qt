#include "Game.h"
#include "GridScene.h"
#include "QTimer"
#include "qevent.h"
#include "qscrollbar.h"

Game::Game(QWidget *parent)
{
    QString buttonStyle = R"(
    QPushButton {
        background-color: #6272a4;
        color: #f8f8f2;
        border-radius: 8px;
        font-weight: 500;
        font-size: 16px;
        padding: 4px 8px;
    }
    QPushButton:hover {
        background-color: #44475a;
    }
    QPushButton:pressed {
        background-color: #3b3d4d;
    }
    QPushButton:disabled {
        background-color: #282a36;
        color: #7a7a7a;
    }
)";



    scene = new GridScene(this);
    setScene(scene);

    scene->setSceneRect(0,0, 20064*2, 20064*2);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // QPixmap pixmap(32, 32);
    // pixmap.fill(Qt::transparent); // tło komórki
    // QPainter painter(&pixmap);
    // //painter.setPen(QPen(QColor(200, 200, 200, 50), 1)); // Jasnoszara siatka, lekko przezroczysta
    // painter.setPen(QPen(Qt::lightGray, 0.5));
    // painter.drawRect(0, 0, 31, 31);
    // painter.end();


    // scene->setBackgroundBrush(pixmap);

    centerOn(scene->sceneRect().center());

    start = new QPushButton("Start", this);
    start->setFixedSize(250, 60);  // ustalamy stały rozmiar

    step = new QPushButton("Step", this);
    step->setFixedSize(200, 40);  // ustalamy stały rozmiar

    clear = new QPushButton("Clear", this);
    clear->setFixedSize(200, 40);  // ustalamy stały rozmiar

    speed = new QPushButton("Speed x1", this);
    speed->setFixedSize(100, 40);  // ustalamy stały rozmiar

    connect(start, &QPushButton::clicked, this, &Game::startStop);
    connect(step, &QPushButton::clicked, this, &Game::move);
    connect(clear, &QPushButton::clicked, this, &Game::clearScene);
    connect(speed, &QPushButton::clicked, this, &Game::speedControl);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::move);
    //timer->start(200); // 200 ms


    // w konstruktorze Game
    pp = new PatternPanel(this, this); // dziecko QGraphicsView
    pp->move(10, height() - pp->height() - 10); // dolny lewy róg


    //ppp->loadFile();

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    mainMenu = new MainMenu(this, this);

    //mainMenu->move(40,40);
    mainMenu->setVisible(true);
    //setEnabled(false);
    //mainMenu->setEnabled(true);

    start->setStyleSheet(buttonStyle);
    step->setStyleSheet(buttonStyle);
    clear->setStyleSheet(buttonStyle);
    speed->setStyleSheet(buttonStyle);

}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_P) {
        pp->togglePanel();
    }

    QGraphicsView::keyPressEvent(event);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){

        panning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    if(event->button() == Qt::RightButton){
        QPointF scenePos = mapToScene(event->pos());
        if(!ifSquareHere(scenePos)){
            qDebug() << "dodano";
            addSquare(scenePos);
        } else{
            removeSquareByPos(scenePos);
        }
        qDebug() << ifSquareHere(scenePos);
    }
    QGraphicsView::mousePressEvent(event);
}



void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(panning){
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();


        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        panning = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void Game::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    const double minScale = 0.1;
    const double maxScale = 5.0;

    double currentScale = transform().m11();
    double factor = event->angleDelta().y() > 0
                        ? scaleFactor
                        : 1.0 / scaleFactor;

    if ((currentScale * factor < minScale && factor < 1) ||
        (currentScale * factor > maxScale && factor > 1))
        return;

    scale(factor, factor);
}





void Game::addSquare(QPointF pos)
{
    int x = (pos.x()/32);
    int y = (pos.y()/32);


    Square * sq = new Square();
    scene->addItem(sq);
    sq->setPos(x*32, y*32);
    //qDebug() << pos;

    aliveSquares.push_back(sq);
    aliveSquaresPos.push_back(sq->scenePos());
}

bool Game::ifSquareHere(QPointF pos)
{
    int x = (pos.x()/32);
    int y = (pos.y()/32);

    int xr = x*32;
    int yr = y*32;

    for(auto e: aliveSquaresPos){
        if(e == QPointF(xr, yr)){
            return true;
        }
    }
    return false;
}

void Game::removeSquareByPos(QPointF pos)
{
    QList<QGraphicsItem*> itemsAtPos = scene->items(pos);

    if(!itemsAtPos.isEmpty()){
        QGraphicsItem* topItem = itemsAtPos.first();

        if(Square* sq = dynamic_cast<Square*>(topItem)){
            aliveSquares.removeOne(sq);
            aliveSquaresPos.removeOne(sq->scenePos());
            scene->removeItem(sq);
            delete sq;
            qDebug() << "Usunięto kwadrat pod pozycją" << pos;
        }
    }
}

QPoint Game::toGridPos(const QPointF& scenePos)
{
    return QPoint(scenePos.x() / 32, scenePos.y() / 32);
}

QPointF Game::gridToScene(const QPoint& p)
{
    return QPointF(p.x() * 32, p.y() * 32);
}


void Game::move()
{
    QHash<QPoint, int> neighborCount;

    //Zliczanie sąsiadów
    for (Square* sq : aliveSquares)
    {
        QPoint p = toGridPos(sq->scenePos());

        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0) continue;
                neighborCount[QPoint(p.x()+dx, p.y()+dy)]++;
            }
    }

    QVector<QPoint> nextAlive;

    //Reguły życia
    for (auto it = neighborCount.begin(); it != neighborCount.end(); ++it)
    {
        QPoint cell = it.key();
        int count = it.value();

        bool alive = ifSquareHere(gridToScene(cell));

        if ((alive && (count == 2 || count == 3)) ||
            (!alive && count == 3))
        {
            nextAlive.push_back(cell);
        }
    }

    //Usuń
    for (Square* sq : aliveSquares)
    {
        scene->removeItem(sq);
        delete sq;
    }
    aliveSquares.clear();
    aliveSquaresPos.clear();

    //Dodaj nowe
    for (const QPoint& p : nextAlive)
    {
        addSquare(gridToScene(p));
    }
}

void Game::startStop()
{
    if(running == false){
        timer->start(interval);
        running= true;
        start->setText("Stop");
    } else{
        timer->stop();
        running = false;
        start->setText("Start");
    }
}

void Game::clearScene()
{
    for (Square* sq : aliveSquares)
    {
        scene->removeItem(sq);
        delete sq;
    }
    aliveSquares.clear();
    aliveSquaresPos.clear();
}

void Game::speedControl()
{
    speedIndex++;

    if (speedIndex >= speedIntervals.size())
        speedIndex = 0;

    interval = speedIntervals[speedIndex];
    timer->setInterval(interval);

    int speedMultiplier = 1000 / interval;
    speed->setText("Speed x" + QString::number(speedMultiplier));
}



void Game::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int spacing = 10;
    int totalWidth = start->width() + spacing + step->width() + spacing + clear->width();


    int x = (width() - totalWidth) / 2;
    int y = height() - step->height() - 10; // 10 px od dołu


    clear->move(x, y);
    start->move(x + clear->width() + spacing, y-25);
    step->move(x + clear->width() + start->width() + 2*spacing, y);


    int margin = 10;

    int xS = width() - speed->width() - margin;
    int yS = height() - speed->height() - margin;

    speed->move(xS, yS);

    pp->move(10, height() - pp->height() - 10);

    int xM = (width()  - mainMenu->width())  / 2;
    int yM = (height() - mainMenu->height()) / 2;

    mainMenu->move(xM, yM);

}

