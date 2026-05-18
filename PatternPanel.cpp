#include "PatternPanel.h"
#include <QFile>
#include <QVector>
#include "Game.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>


PatternPanel::PatternPanel(Game *game, QWidget *parent) : QWidget(parent),
    m_game(game)
{
    setFixedSize(250, 300);
    setStyleSheet("background-color: rgba(20,20,20,220); border: 1px solid gray;");

    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel *title = new QLabel("Pattern Panel", this);
    title->setStyleSheet("color: white; font-weight: bold;");
    layout->addWidget(title);

    QLineEdit *searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Szukaj...");
    searchEdit->setStyleSheet("color: white;");
    layout->addWidget(searchEdit);

    connect(searchEdit, &QLineEdit::textChanged,
        this, [=](const QString &text)
        {
            for (int i = 0; i < listWidget->count(); ++i)
            {
                QListWidgetItem *item = listWidget->item(i);
                bool match = item->text().contains(text, Qt::CaseInsensitive);
                item->setHidden(!match);
            }
        }
    );


    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);


    listWidget->setStyleSheet(
        "QListWidget { "
        "    background-color: rgba(24,24,24,200); " // ciemne tło listy
        "} "
        "QListWidget::item { "
        "    color: #E0E0E0; "                     // jasny tekst
        "} "
        "QListWidget::item:selected { "
        "    background-color: rgba(72,61,139,180); " // fioletowy przy zaznaczeniu
        "} "
        "QScrollBar:vertical { "
        "    background: #282828; " // ciemny pasek
        "    border-radius: 6px; "
        "    margin: 0px; "
        "} "
        "QScrollBar::handle:vertical { "
        "    background-color: #7F7FFF; "           // niebiesko-fioletowy uchwyt
        "    border-radius: 6px; "
        "} "
        "QScrollBar::handle:vertical:hover { "
        "    background-color: #AFAFFF; "           // jaśniejszy po najechaniu
        "} "
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, "
        "QScrollBar::sub-page:vertical, QScrollBar::add-page:vertical { "
        "    height: 0px; "
        "    background: none; "         // brak przycisków
        "} "
        "QScrollBar:horizontal { "
        "    background: #282828;  " // tło poziomego paska
        "    height: 12px; "
        "    border-radius: 6px; "
        "    margin: 0px; "
        "} "
        "QScrollBar::handle:horizontal { "
        "    background-color: #7F7FFF; "
        "    border-radius: 6px; "
        "} "
        "QScrollBar::handle:horizontal:hover { "
        "    background-color: #AFAFFF; "
        "} "
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, "
        "QScrollBar::sub-page:horizontal, QScrollBar::add-page:horizontal { "
        "    width: 0px; "
        "    background: none; "
        "}"
        );



    // pattern panel
    setStyleSheet(
        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.5, y2:1, stop:0 #282a36, stop:1 #1e1f29);"
        "border: 2px solid #6272a4;"
        "border-radius: 15px;"
        );

    // tytuł
    title->setStyleSheet(
        "color: #f8f8f2;"
        "font-size: 14px;"
        "font-weight: bold;"
        "border-radius: 10px;"
        "padding: 6px;"
        "text-align: center;"
        );

    // pole wyszukiwania
    searchEdit->setStyleSheet(
        "QLineEdit {"
        "   background-color: #44475a;"
        "   color: #f8f8f2;"
        "   border: 1px solid #6272a4;"
        "   border-radius: 6px;"
        "   padding: 6px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "   background-color: #3b3d4d;"
        "   border: 1px solid #6272a4;"
        "}"
        );

    // lista patternów
    listWidget->setStyleSheet(
        "QListWidget {"
        "   background-color: #44475a;"
        "   color: #f8f8f2;"
        "   border-radius: 10px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #6272a4;"
        "   color: #f8f8f2;"
        "}"
        "QScrollBar:vertical, QScrollBar:horizontal {"
        "   background: #282a36;"
        "   border-radius: 5px;"
        "   width: 10px;"
        "   height: 10px;"
        "}"
        "QScrollBar::handle:vertical, QScrollBar::handle:horizontal {"
        "   background-color: #6272a4;"
        "   border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover {"
        "   background-color: #44475a;"
        "}"
        );








    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);




    setVisible(false); // początkowo schowany

    // po kliknięciu w pattern
    connect(listWidget, &QListWidget::itemClicked, this, &PatternPanel::patternClicked);

    // przykładowe wczytanie listy patternów z katalogu


    // QString appPath = QCoreApplication::applicationDirPath();
    // QString patternsPath = appPath + "/patterns";

    // QDir dir(patternsPath);
    // QStringList patternFiles = dir.entryList(QStringList() << "*.txt", QDir::Files);

    // // patternFiles teraz zawiera wszystkie pliki .txt z folderu
    // for (const QString &file : patternFiles) {
    //     QFileInfo fi(file);
    //     listWidget->addItem(fi.baseName());
    // }

    // for (const QString &file : patternFiles) {
    //     listWidget->addItem(file.split("/").last()); // dodaje tylko nazwę pliku
    // }

    QString appPath = QCoreApplication::applicationDirPath();
    QString patternsPath = appPath + "/life_lexicon_patterns.json";

    QFile file(patternsPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Nie mozna otworzyc pliku:" << patternsPath;
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Blad JSON:" << error.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "JSON nie jest obiektem";
        return;
    }

    QJsonObject root = doc.object();

    if (!root.contains("entries") || !root["entries"].isArray()) {
        qWarning() << "Brak tablicy 'entries' w JSON";
        return;
    }

    entries = root["entries"].toArray();

    qDebug() << "Liczba kluczy:" << entries.size();

    for (const QJsonValue& val : entries) {
        if (!val.isObject()) continue;
        QJsonObject entry = val.toObject();
        QString key = entry["name"].toString();
        //qDebug() << key;
        listWidget->addItem(key);
    }



}

void PatternPanel::patternClicked(QListWidgetItem *item)
{
    if (!item) return;

    QVector <QString> p;

    // QString fileName = item->text() + ".txt";

    // QString appPath = QCoreApplication::applicationDirPath();
    // QString patternsPath = appPath + "/patterns/";

    // // zakładam, że wszystkie pliki w tym samym folderze
    // QString fullPath = patternsPath + fileName;
    // loadFile(fullPath);

    QString keyToFind = item->text();

    for (const QJsonValue& val : entries) {
        if (!val.isObject()) continue;
        QJsonObject entry = val.toObject();

        if (entry["name"].toString() == keyToFind) {
            if (entry.contains("pattern") && entry["pattern"].isArray()) {
                QJsonArray patternArray = entry["pattern"].toArray();
                for (const QJsonValue& lineVal : patternArray) {
                    QString line = lineVal.toString();
                    //qDebug() << line; // wypisuje każdy wiersz pattern
                    p.push_back(line);
                }
            }
            break; // znaleziono klucz, kończymy
        }
    }

    loadPattern(p);

}

void PatternPanel::togglePanel()
{
    m_visible = !m_visible;
    setVisible(m_visible);
}


void PatternPanel::loadFile(QString &filePath)
{
    m_game->clearScene();

    QVector <QString> pattern;

    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku";
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << line;
        pattern.push_back(line);
    }

    int patternWidth  = pattern[0].size() * 32;
    int patternHeight = pattern.size() * 32;

    QRectF r = m_game->scene->sceneRect();

    qreal sceneCenterX = r.center().x();
    qreal sceneCenterY = r.center().y();

    qreal offsetX = sceneCenterX - patternWidth  / 2;
    qreal offsetY = sceneCenterY - patternHeight / 2;




    for(int i = 0; i < pattern.size(); i++){
        for(int j = 0; j < pattern[i].size(); j++){
            if(pattern[i][j] == "O"){
                //qDebug() << i << " " << j;


                //m_game->addSquare(QPointF((32*j + m_game->scene->width()/2),32*i + m_game->scene->height()/2));

                m_game->addSquare(QPointF(offsetX + j * 32, offsetY + i * 32));
            }
        }
    }



}

void PatternPanel::loadPattern(QVector<QString> pattern)
{
    m_game->clearScene();

    int patternWidth  = pattern[0].size() * 32;
    int patternHeight = pattern.size() * 32;

    QRectF r = m_game->scene->sceneRect();

    qreal sceneCenterX = r.center().x();
    qreal sceneCenterY = r.center().y();

    qreal offsetX = sceneCenterX - patternWidth  / 2;
    qreal offsetY = sceneCenterY - patternHeight / 2;




    for(int i = 0; i < pattern.size(); i++){
        for(int j = 0; j < pattern[i].size(); j++){
            if(pattern[i][j] == "O"){
                //qDebug() << i << " " << j;


                //m_game->addSquare(QPointF((32*j + m_game->scene->width()/2),32*i + m_game->scene->height()/2));

                m_game->addSquare(QPointF(offsetX + j * 32, offsetY + i * 32));
            }
        }
    }
}
