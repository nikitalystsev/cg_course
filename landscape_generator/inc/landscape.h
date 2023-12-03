#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "light.h"
#include "point/point3D.h"
#include "transform.h"
#include "vector/vector3D.h"
#include "zbuffer.h"
#include <QGraphicsScene>
#include <QPainter>
#include <cstdarg>
#include <utility>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

class Landscape
{
private:
    Matrix<Point3D<double>> _map; // карта высот
    int _width, _lenght;          // длина и ширина карты высот (в количестве полигонов)
    int _waterlevel;              // уровень воды

    ZBuffer _zBuffer; //  удаление невидимых линий
    void _calcZBuffer(const Matrix<Point3D<double>> &screenMap);

    Point3D<double> _centerPoint; // центральная точка всего ландшафта
    void _calcCenterPoint();

    Light _light;                                                // источник освещения
    Matrix<pair<Vector3D<double>, Vector3D<double>>> _normalMap; // матрица векторов внешней нормали к каждой из граней
    Matrix<Vector3D<double>> _normalVertexMap;                   // матрица векторов нормалей для каждой вершины
    Matrix<double> _intensityVertexMap;                          // матрица интенсивностей света для каждой вершины
    void _calcNormalForEachPlane();                              // расчет нормалей для каждой грани
    void _calcNormalForEachVertex();                             // расчет нормалей для каждой вершины
    void _calcIntensityForEachVertex();                          // расчет интенсивности в каждой вершине
    void _calcFramebuffer(const Matrix<Point3D<double>> &screenMap);

    // методы для визуализации изображения
    Matrix<Point3D<double>> _mapToScreen();
    void _drawMap(QGraphicsScene *scene) const;
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
