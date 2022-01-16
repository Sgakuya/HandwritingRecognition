#include "mainwindow.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

#include "qdrawspace.h"
#include "character.h"

/* constructor for MainWindow: derived from QMainWindow */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *widget = new QWidget();
    setCentralWidget(widget);
    QVBoxLayout *layout = new QVBoxLayout();
    widget->setLayout(layout);
    clear = new QPushButton("Clear Screen");
    drawSpace = new QDrawSpace();
    drawSpace->setMinimumHeight(200.0);
    drawSpace->setMaximumHeight(200.0);
    drawSpace->fitInView(QRectF(-150.0, -150.0, 300.0, 300.0), Qt::KeepAspectRatioByExpanding);
    console = new QTextEdit();
    layout->addWidget(clear);
    layout->addSpacing(3);
    layout->addWidget(drawSpace, 1);
    layout->addSpacing(20.0);
    layout->addWidget(console, 5);
    connect(clear, &QPushButton::clicked, this, &MainWindow::clearScreen);
    connect(drawSpace, &QDrawSpace::newShape, this, &MainWindow::newShapeDrawn);
    connect(drawSpace, &QDrawSpace::newLetter, this, &MainWindow::newLetterDrawn);
    debugMode = !true;
}

/* destructor for MainWindow */
MainWindow::~MainWindow() {}

/* clears the writing panel and console */
void MainWindow::clearScreen() {
    drawSpace->clear();
    console->clear();
}

/* debug mode: outputs information about the drawn objects on the console i.e. vertices, time stamps, displacements etc */
void MainWindow::newShapeDrawn(QString s) { if (debugMode) console->append(s); }

/* calls function to read the letter segments and outputs the letter to the console */
void MainWindow::newLetterDrawn(Character c) {
    QString s = c.decipherLetter();
    if (!debugMode) {
        console->moveCursor(QTextCursor::End);
        console->insertPlainText(s);
        console->moveCursor(QTextCursor::End);
    }
    s.clear();
}
