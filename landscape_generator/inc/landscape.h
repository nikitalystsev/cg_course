#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "point/point3D.h"
#include "transform.h"
#include "zbuffer.h"
#include <QGraphicsScene>
#include <QPainter>
#include <vector>

template <typename T>
using vector = std::vector<T>;

class Landscape
{
private:
    vector<vector<Point3D<double>>> _map;
    int _width, _lenght; // в количестве полигонов
    int _waterlevel;
    ZBuffer zBuffer; // класс алгоритма zbuffer

    Point3D<double> _centerPoint;

private:
    vector<vector<Point3D<double>>> _mapToScreen();
    void _calcZBuffer(const vector<vector<Point3D<double>>> &screenMap);
    void _drawMap(QGraphicsScene *scene) const;
    void _calcCenterPoint();
    void _shiftPointToOrigin(Point3D<double> &point);
    void _shiftPointBackToOrigin(Point3D<double> &point);
    void _movePointToCenter(Point3D<double> &point);

public:
    Landscape();
    Landscape(const int width, const int lenght, const int waterlevel);
    ~Landscape();

    void generateHeightMap();
    void draw(QGraphicsScene *scene);

    int getWaterlevel() const;
    void setWaterlevel(const int waterlevel);

    Point3D<double> getCenterPoint() const;
    void setCenterPoint(const Point3D<double> &centerPoint);

public:
    static const int default_width = 100, default_lenght = 100;
    static const int default_waterlevel = 100;

    static const int poly_size = 5;
};

#endif
