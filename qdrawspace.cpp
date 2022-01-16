#include "qdrawspace.h"
#include "mainwindow.h"

#include <QSurface>
#include <QDebug>
#include <QtWidgets>
#include <QFile>
#include <QGraphicsLineItem>
#include <QDebug>

/* constructor for QDrawSpace */
QDrawSpace::QDrawSpace() {
    setScene(&mScene);
    setSceneRect(QRectF(-4.25*100.0, -0.35*100.0, 8.5*100.0, 0.7*100.0));
    setRenderHint(QPainter::Antialiasing);
    mouseIsDown = false;
    setMouseTracking(true);
    setRightX(-1000000.0);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    makeWritingLines();
    writingLines->setPos(0.0, 0.0);
    writingLines->setVisible(true);
}

/* destructor for QDrawSpace */
QDrawSpace::~QDrawSpace() {}

/* draws the writing lines on the widget containing the writing panel */
void QDrawSpace::makeWritingLines() {
    double delY = 0.25*100.0;
    double w =  8.25*100.0;
    writingLines = new QGraphicsItemGroup();
    QPen midPen(Qt::red, 1.0);
    midPen.setDashPattern(QVector<qreal>() << 2.0 << 2.0);  
    QGraphicsLineItem *midLine = new QGraphicsLineItem(-w, 0.0, w, 0.0);
    midLine->setPen(midPen);
    midLine->setOpacity(0.5);
    writingLines->addToGroup(midLine);
    QPen topBotPen(Qt::blue, 2.0);
    QGraphicsLineItem *topLine = new QGraphicsLineItem(-w, delY, w, delY);
    topLine->setPen(topBotPen);
    topLine->setOpacity(0.5);
    writingLines->addToGroup(topLine);
    QGraphicsLineItem *botLine = new QGraphicsLineItem(-w, -delY, w, -delY);
    botLine->setPen(topBotPen);
    botLine->setOpacity(0.5);
    writingLines->addToGroup(botLine);
    writingLines->setVisible(false);
    writingLines->setZValue(200.0);
    scene()->addItem(writingLines);
}

/* updates information about object when mouse is clicked */
void QDrawSpace::mousePressEvent(QMouseEvent *evt) {
    QGraphicsView::mousePressEvent(evt);
    mouseIsDown = true;
    QPointF pos = mapToScene(evt->pos());
    lastPos = pos;
    currentDrawnObject.addData(pos, evt->timestamp());
    lastPos = pos;
}

/* adds a segment if the object has been extended since last recorded position */
void QDrawSpace::maybeAddSegment(const QPointF &pos) {
    if (lastPos != pos) {
        QGraphicsLineItem *item = mScene.addLine(QLineF(lastPos, pos), QPen(Qt::gray, 2.0));
        item->setZValue(100.0);
        lastPos = pos;
    }
}

void QDrawSpace::mouseReleaseEvent(QMouseEvent *evt) {
    if (!mouseIsDown) return;
    mouseIsDown = false;
    QPointF pos = mapToScene(evt->pos());
    QGraphicsView::mouseReleaseEvent(evt);
    currentDrawnObject.addData(pos, evt->timestamp());
    currentDrawnObject.findVertices(8);
    for (MarkedPoint p : currentDrawnObject.vertices) {
        QGraphicsEllipseItem *dot = scene()->addEllipse(-2.0, -2.0, 4.0, 4.0, Qt::NoPen, Qt::red);
        dot->setZValue(1000.0);
        dot->setPos(p.pos);
    }
    QString s;
    QTextStream out(&s);
    out << "Vertices\n";
    currentDrawnObject.trace(out, currentDrawnObject.vertices);
    currentDrawnObject.getRangeX();
    for (unsigned int i = 0; i < currentDrawnObject.vertices.size()-1; ++i) {
        auto disps = currentDrawnObject.normalDisplacements(i);
        out << disps.first << " / " << disps.second << "\n";
    }
    emit newShape(s);
    if (objects.size() >= 1) {
        if (rightMostX >= currentDrawnObject.leftMostX) objects.push_back(currentDrawnObject);
        else {
            emit newLetter(Character(objects));
            objects.clear();
            objects.push_back(currentDrawnObject);
        }
    }else
        objects.push_back(currentDrawnObject);
    setRightX(currentDrawnObject.rightMostX);
    currentDrawnObject.clear();
}

/* keeps track of whether a new segment should be added while adding data to current object */
void QDrawSpace::mouseMoveEvent(QMouseEvent *evt) {
    QGraphicsView::mouseMoveEvent(evt);
    if (mouseIsDown) {
        QPointF pos = mapToScene(evt->pos());
        currentDrawnObject.addData(pos, evt->timestamp());
        maybeAddSegment(pos);
    }
}

/* clears the drawspace when clear screen button is clicked */
void QDrawSpace::clear() {
    mScene.clear();
    makeWritingLines();
    writingLines->setPos(0.0, 0.0);
    writingLines->setVisible(true);
    mScene.update();
    setRightX(-1000000.0);
}

/* setter for rightMostX value */
void QDrawSpace::setRightX(int i) {
    rightMostX = i;
}


