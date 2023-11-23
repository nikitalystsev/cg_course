#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "point/point3D.h"
#include "transform.h"
#include "vector/vector3D.h"
#include "zbuffer.h"
#include <QGraphicsScene>
#include <QPainter>
#include <utility>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

class Landscape
{
private:
    vector<vector<Point3D<double>>> _map;
    int _width, _lenght; // в количестве полигонов
    int _waterlevel;
    ZBuffer zBuffer; // класс алгоритма zbuffer

    Point3D<double> _centerPoint;

    // матрица векторов внешней нормали к каждой из граней
    vector<vector<pair<Vector3D<double>, Vector3D<double>>>> _normalMap;
    // матрица векторов нормалей для каждой вершины
    vector<vector<Vector3D<double>>> _normalVertexMap;

private:
    vector<vector<Point3D<double>>> _mapToScreen();
    // методы для z-буффера
    void _calcZBuffer(const vector<vector<Point3D<double>>> &screenMap);
    // методы для визуализации изображения
    void _drawMap(QGraphicsScene *scene) const;
    void _calcCenterPoint();
    void _shiftPointToOrigin(Point3D<double> &point);
    void _shiftPointBackToOrigin(Point3D<double> &point);
    void _movePointToCenter(Point3D<double> &point);

    // методы работы с освещением и закраской
    void _caclNormalForEachPlane(const vector<vector<Point3D<double>>> &screenMap);
    void _caclNormalForEachVertex(const vector<vector<Point3D<double>>> &screenMap);

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
