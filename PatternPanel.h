#ifndef PATTERNPANEL_H
#define PATTERNPANEL_H

//#include "Game.h"
#include "qboxlayout.h"
#include "qwidget.h"
#include <QGraphicsItem>
#include <QObject>
#include <QListWidgetItem>
#include <QJsonArray>

class Game;

class PatternPanel: public QWidget {
    Q_OBJECT
public:
    PatternPanel(Game *game,QWidget *parent = nullptr);

    void loadFile(QString &filePath);

    void loadPattern(QVector <QString> pattern);

    QJsonArray entries;

    void togglePanel();

    bool m_visible = false;

private slots:
    void patternClicked(QListWidgetItem *item);
private:
    Game *m_game;
    QVBoxLayout *layout;
    QListWidget *listWidget;

};

#endif // PATTERNPANEL_H
