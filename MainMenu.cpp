#include "MainMenu.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "Game.h"

MainMenu::MainMenu(Game *game, QWidget *parent) : QWidget(parent),
    m_game(game){

    setAttribute(Qt::WA_StyledBackground, true);


    setFixedSize(640, 440);
    setStyleSheet(
        "background-color: rgba(20,20,20,230);"
        "border: 1px solid gray;"
        "border-radius: 12px;"
        );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(18);

    // ===== Tytuł =====
    QLabel *title = new QLabel("GAME OF LIFE", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "color: #7FFF7F;"
        "font-size: 28px;"
        "font-weight: bold;"
        );
    layout->addWidget(title);

    layout->addStretch();

    // ===== Zasady =====
    QLabel *rules = new QLabel(
        "Zasady gry:\n\n"
        "• < 2 sąsiadów → śmierć\n"
        "• 2–3 sąsiadów → życie\n"
        "• > 3 sąsiadów → śmierć\n"
        "• 3 sąsiadów → narodziny"
        );
    rules->setWordWrap(true);
    rules->setStyleSheet(
        "color: #E0E0E0;"
        "background-color: rgba(40,40,40,180);"
        "padding: 12px;"
        "border-radius: 8px;"
        );
    layout->addWidget(rules);

    QLabel *panel = new QLabel(
        "Aby otworzyć panel z szablonami wcisnij P."
        );
    panel->setWordWrap(true);
    panel->setStyleSheet(
        "color: #E0E0E0;"
        "background-color: rgba(40,40,40,180);"
        "padding: 12px;"
        "border-radius: 8px;"
        );
    layout->addWidget(panel);

    layout->addStretch();

    // ===== Start =====
    QPushButton *startBtn = new QPushButton("START", this);
    startBtn->setFixedHeight(42);
    startBtn->setCursor(Qt::PointingHandCursor);
    startBtn->setStyleSheet(
        "QPushButton {"
        "  color: white;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "  background-color: #4CAF50;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #6EE06E;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #3B8C3B;"
        "}"
        );

    connect(startBtn, &QPushButton::clicked, this, &MainMenu::start);

    layout->addWidget(startBtn);

    // main menu
    setStyleSheet(
        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.5, y2:1, stop:0 #282a36, stop:1 #1e1f29);"
        "border: 2px solid #6272a4;"
        "border-radius: 15px;"
        );

    // tytuł
    title->setStyleSheet(
        "color: #f8f8f2;"
        "font-size: 28px;"
        "font-weight: bold;"
        "padding: 8px;"
        "text-align: center;"
        );

    // zasady
    rules->setStyleSheet(
        "color: #f8f8f2;"
        "background-color: #44475a;"
        "padding: 12px;"
        "border-radius: 10px;"
        );

    // przycisk START
    startBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #6272a4;"
        "   color: #f8f8f2;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "   font-size: 16px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover { background-color: #44475a; }"
        "QPushButton:pressed { background-color: #3b3d4d; }"
        );




}

void MainMenu::start()
{
    m_game->mainMenu->setVisible(false);
    m_game->setEnabled(true);
}
