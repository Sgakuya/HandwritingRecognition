#ifndef QDRAWSPACE_H
#define QDRAWSPACE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
#include <cmath>
#include <vector>
#include <list>

#include "drawnobject.h"
#include "character.h"


class QDrawSpace : public QGraphicsView {
    Q_OBJECT
    QPointF lastPos;
    QPointF center0, pinchPos0;
    QGraphicsItemGroup *writingLines;

    double viewportScale0, scale0;

public:
    QGraphicsScene mScene;
    DrawnObject currentDrawnObject;
    DrawnObject freehandObject;
    bool mouseIsDown;
    double rightMostX;

    std::list<DrawnObject> objects;
    std::vector<MarkedPoint> letterVertices;

    void setRightX(int i);
    void maybeAddSegment(const QPointF &pos);
    void drawExisting();
    void makeWritingLines();

public:
    QDrawSpace();
    ~QDrawSpace();

    void setFreeDrawMode(bool _freeDrawMode);
    void clear();

protected:
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void mouseMoveEvent(QMouseEvent *evt) override;

signals:
    void newShape(QString s);
    void newLetter(Character c);
};

#endif // QDRAWSPACE_H
