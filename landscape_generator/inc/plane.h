#ifndef PLANE_H
#define PLANE_H

#include "point/point2D.h"
#include "point/point3D.h"

#include <iostream>

class Plane
{
private:
    Point3D<double> _p1, _p2, _p3; // точки плоскости на экране
    double A, B, C, D;             // коэф-ты плоскости (они же координаты внешней нормали)

    // точки описывающего прямоугольника
    Point2D<double> _pMin, _pMax;

    void _calcPlaneCoeff();
    // метод, определяющий координаты описывающего треугольную плоскость прямоугольника
    void _defCircumRect();

public:
    Plane(const Point3D<double> &p1, const Point3D<double> &p2, const Point3D<double> &p3);

    double caclZ(const double x, const double y);

    /* Cторона А - на точках _p1 _p2
     * Cторона B - на точках _p3 _p3
     * Cторона C - на точках _p3 _p1
     */

    int equASide(const int x, const int y);
    int equBSide(const int x, const int y);
    int equCSide(const int x, const int y);

    Point3D<double>
    getP1() const;
    Point3D<double> getP2() const;
    Point3D<double> getP3() const;

    Point2D<double> getPMin() const;
    Point2D<double> getPMax() const;

    double getA() const;
    double getB() const;
    double getC() const;
    double getD() const;
};

#endif
