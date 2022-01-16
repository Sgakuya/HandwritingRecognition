#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drawnobject.h>
#include <character.h>

class QDrawSpace;
class QTextEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

    QDrawSpace *drawSpace;
    QTextEdit *console;
    QPushButton *clear;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void clearScreen();
    QString currWord;
    bool debugMode;

public slots:
    void newShapeDrawn(QString s);
    void newLetterDrawn(Character c);
};
#endif // MAINWINDOW_H
