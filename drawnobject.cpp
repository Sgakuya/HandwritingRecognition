#include "drawnobject.h"
#include <QWidget>
#include <stdio.h>
#include <QtMath>
#include <cstdlib>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QDebug>
#include <QVector2D>
#include <QTextStream>
#include <cstdlib>
#include <QFile>

#include "qdrawspace.h"

using namespace std;

MarkedPoint::MarkedPoint(QPointF _pos, int _time, int _index):pos(_pos),time(_time),index(_index) {}
MarkedPoint::MarkedPoint(const MarkedPoint &mp, int _index):pos(mp.pos),time(mp.time),index(_index) { }

/* outputs information about the current DrawnObject to text stream out */
void DrawnObject::trace(QTextStream &out, vector<MarkedPoint> &points) {
    for (unsigned int i = 0; i < points.size(); ++i) {
        out << points[i].pos.x() << "\t" << points[i].pos.y() << "\t" << points[i].time;
        if (i) {
            int dx = points[i].pos.x()-points[i-1].pos.x(), dy = points[i].pos.y()-points[i-1].pos.y(), dt = points[i].time-points[i-1].time;
            out << "\t" << dx << "\t" << dy << "\t" << dt << "\t" << ((abs(dx)+abs(dy)==1 && dt==0) ? "X" : "");
        }
        out << "\n";
    }
    out << "=====\n";
}

DrawnObject::DrawnObject(QPen _pen) {
    inputPoints.reserve(1000);
    vertices.reserve(100);
    setPen(_pen);
    rightMostX = -1000000.0;
    leftMostX = 1000000.0;
    rangeX = 0.0;
}

void DrawnObject::setPen(QPen _pen) {
    pen = _pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    clearPen = pen;
    clearPen.setWidthF(pen.widthF()*6.0);
    clearPen.setBrush(Qt::white);
}

/* checks if object is a dot */
bool DrawnObject::isDot() const { return vertices.size()==1; }

/* checks if object is a segment */
bool DrawnObject::isSegment() const { return vertices.size()==2; }

/* checks if object is curved */
bool DrawnObject::isCurved(double l, double r) const {
    if (vertices.size()==1) return false;
    if (l >= 0.09 || r >= 0.09) return true;
    return false;
}

/* checks if object is straight */
bool DrawnObject::isStraight(double l, double r) const {
    if (vertices.size()==1) return false;
    if (l <= 0.09 || r <= 0.09) return true;
    return false;
}

/* returns left and right most X values of current drawn object */
void DrawnObject::getRangeX(){
    for (unsigned int i = 0; i < inputPoints.size(); ++i){
        if (inputPoints[i].pos.x() > rightMostX) setRightX(inputPoints[i].pos.x());
        if (inputPoints[i].pos.x() < leftMostX) setLeftX(inputPoints[i].pos.x());
    }
    rangeX = rightMostX - leftMostX;
    setRangeX(rangeX);
}

/* returns a numeric value for the displacement from the shortest path between two vertices */
pair<double, double> DrawnObject::normalDisplacements(int j) {
    int i0 = vertices[j].index, i1 = vertices[j+1].index;
    QVector2D p0(inputPoints[i0].pos);
    QLineF line(inputPoints[i0].pos, inputPoints[i1].pos);
    QVector2D n(QLineF::fromPolar(1.0, line.angle()+90.0).pointAt(1.0));
    double minAbsDisp = 0.0, maxAbsDisp = 0.0;
    for (int i = i0; ++i < i1; ) {
        double disp = QVector2D::dotProduct(n, QVector2D(inputPoints[i].pos) - p0);
        if (disp > maxAbsDisp) maxAbsDisp = disp;
        if (-disp > minAbsDisp) minAbsDisp = -disp;
    }
    double dist = line.length();
    if(dist == 0.0 ) dist = 1;
    return make_pair(minAbsDisp/dist, maxAbsDisp/dist);
}

void DrawnObject::setPath() {
    path.clear();
    vector<MarkedPoint> &points = inputPoints;
    if (isDot()) {
        double r = 0.5*pen.widthF();
        path.addEllipse(QRectF(points[0].pos - QPointF(r, r), QSizeF(2.0*r, 2.0*r)));
    } else {
        for (unsigned int i = 0; i < points.size(); ++i) {
            if (i==0) path.moveTo(points[i].pos);
            else path.lineTo(points[i].pos);
        }
    }
    findConvexHull(inputPoints);
}

void DrawnObject::findConvexHull(std::vector<MarkedPoint> &points) {
    hull.clear();
    int i, nPoints = points.size();
    QPointF botPoint(0.0, 0.0);
    for (i = 0; i < nPoints; ++i) {
        double dx = points[i].pos.x() - botPoint.x(), dy = points[i].pos.y() - botPoint.y();
        if (i==0 || dy < 0.0 || (dy==0.0 && dx > 0.0)) botPoint = points[i].pos;
    }
    QPointF p0 = botPoint;
    hull.push_back(MarkedPoint(p0));
    QLineF line0(p0, p0+QPointF(1.0,0.0));
    do {
        double bestAngle = 360.0, bestLength = 0.0;
        MarkedPoint bestPoint(p0);
        for (i = 0; i < nPoints; ++i) if (points[i].pos!=p0) {
            QLineF testLine(p0, points[i].pos);
            double angle = line0.angleTo(testLine), length = testLine.length();
            if (angle < bestAngle || (angle==bestAngle && length > bestLength)) {
                bestPoint = points[i];
                bestAngle = angle;
                bestLength = length;
            }
        }
        hull.push_back(bestPoint);
        line0 = QLineF(p0, bestPoint.pos);
        p0 = bestPoint.pos;
    } while (hull.back().pos!=botPoint);
    hull.pop_back();
}


void DrawnObject::drawIn(QGraphicsScene &scene) {
    QGraphicsPathItem *item = scene.addPath(path, pen);
    item->setZValue(11.0);
}

void DrawnObject::preDrawIn(QGraphicsScene &scene) {
    setPath();
    QPainterPath hullPath;
    if (isDot()) {
        hullPath.clear();
        double r = 2.0*pen.widthF();
        hullPath.addEllipse(QRectF(vertices[0].pos - QPointF(r, r), QSizeF(2.0*r, 2.0*r)));
    } else {
        int i = 0;
        for (MarkedPoint &mp : hull){
            if (i++==0) hullPath.moveTo(mp.pos);
            else hullPath.lineTo(mp.pos);
        }
        hullPath.closeSubpath();
    }
    QGraphicsPathItem *item = scene.addPath(hullPath, clearPen, QBrush(Qt::white));
    item->setZValue(11.0);
}

/* adds data to the current DrawnObject */
void DrawnObject::addData(QPointF pos, int data) { inputPoints.push_back(MarkedPoint(pos, data, inputPoints.size())); }

/* clears information from current DrawnObject */
void DrawnObject::clear() {
    vertices.clear();
    inputPoints.clear();
    leftMostX = 10000000.0;
    rightMostX = -10000000.0;
}

void DrawnObject::findVertices(double distCutoff) {
    double epsilon = 0.001;
    int i, j, nPoints = inputPoints.size();
    vector<double> X(nPoints, 0.0);
    for (i = 0; i < nPoints; ++i) {
        double ds = QLineF(inputPoints[i-1].pos, inputPoints[i].pos).length();
        double dt = epsilon + inputPoints[i].time - inputPoints[i-1].time;
        double x = 1.0/(epsilon + ds/dt);
        if (i-2 >= 0) X[i-2] += 0.06*x;
        if (i-1 >= 0) X[i-1] += 0.24*x;
        X[i] += 0.6*x;
        if (i+1 < nPoints) X[i+1] = 0.24*x;
        if (i+2 < nPoints) X[i+2] = 0.06*x;
    }
    vector< pair<double, int> > sortedX(nPoints);
    for (j = 0; j < nPoints; ++j)
        sortedX[j] = make_pair(X[j], j);
    sort(sortedX.begin(), sortedX.end());
    double xCutoff = sortedX[0.9*nPoints].first;
    vertices.clear();
    vertices.push_back(inputPoints[0]);
    MarkedPoint *curVertex = &*vertices.rbegin();
    for (i = 0; i < nPoints; ++i) {
        MarkedPoint &testPoint = inputPoints[i];

        if (curVertex) {
            if (QLineF(curVertex->pos, testPoint.pos).length() < distCutoff) {
                if (X[i] > X[curVertex->index])
                    *curVertex = testPoint;
                continue;
            } else
                curVertex = nullptr;
            continue;
        } else if (X[i] >= xCutoff) {
            vertices.push_back(testPoint);
            curVertex = &*vertices.rbegin();
        }
    }
    if (QLineF(vertices.rbegin()->pos, inputPoints.rbegin()->pos).length() > distCutoff)
        vertices.push_back(*inputPoints.rbegin());
    if (QLineF(vertices.begin()->pos, vertices.rbegin()->pos).length() < distCutoff)
        vertices.rbegin()->pos = vertices.begin()->pos;
}

/* setters for leftMostX, rangeX and rightMostX */
void DrawnObject::setLeftX(int i) {leftMostX = i;}
void DrawnObject::setRangeX(int i) {rangeX = i;}
void DrawnObject::setRightX(int i) {rightMostX = i;}

