#ifndef DRAWNOBJECT_H
#define DRAWNOBJECT_H
#include <QWidget>
#include <QPen>
#include <QPainterPath>

class QGraphicsScene;
class QTextStream;

#include <vector>
#include <list>

class MarkedPoint {
public:
    QPointF pos;
    int time, index; // timestamp or original index value of source point
    explicit MarkedPoint(QPointF _pos = QPointF(), int _time = 0, int _index = 0);
    MarkedPoint(const MarkedPoint &mp, int _index);
};

class DrawnObject {
public:
    std::vector<MarkedPoint> inputPoints, vertices;
    std::list<MarkedPoint> hull;
    QPen pen, clearPen;
    QPainterPath path;
    double leftMostX;
    double rightMostX;
    double rangeX;
    //make getters

    DrawnObject(QPen _pen = QPen(Qt::black, 2.0));

    void trace(QTextStream &out, std::vector<MarkedPoint> &points);
    void setPen(QPen _pen);
    void setPath();
    void setRightX(int i);
    void setLeftX(int i);
    void setRangeX(int i);
    void preDrawIn(QGraphicsScene &scene);
    void drawIn(QGraphicsScene &scene);
    void addData(QPointF pos, int data = 0);
    void clear();
    void findVertices(double distCutoff);
    void findConvexHull(std::vector<MarkedPoint> &points);
    void getRangeX();

    bool empty() const;
    bool isDot() const;
    bool isSegment() const;
    bool isCurved(double l, double r) const;
    bool isStraight(double l, double r) const;

    std::pair<double, double> normalDisplacements(int j);
};

#endif // DRAWNOBJECT_H
