#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "../my_matrix_lib/Matrix.hpp"
#include "point.h"
#include <QGraphicsScene>

class Landscape
{
private:
    Matrix<Point3D<double>> _map;
    int _width, _lenght; // в количестве полигонов
    int _waterlevel;

    Matrix<Point3D<double>> _mapToScreen() const;
    void _drawLine(const Point3D<double> &p1,
                   const Point3D<double> &p2,
                   QGraphicsScene *scene) const;
    void _drawMap(const Matrix<Point3D<double>> &screenMap, QGraphicsScene *scene) const;

public:
    Landscape();
    Landscape(const int width, const int lenght);
    ~Landscape();

    void generateHeightMap();
    void draw(QGraphicsScene *scene);

public:
    static const int default_width = 100, default_lenght = 100;
    static const int poly_size = 5;
};

#endif
