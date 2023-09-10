#ifndef PLANE_H
#define PLANE_H

#include "point.h"

class Plane
{
private:
    Point3D<int> _p1, _p2, _p3; // точки плоскости на экране
    double A, B, C, D;          // коэф-ты плоскости

    void _calcPlaneCoeff();

public:
    Plane(const Point3D<int> p1, const Point3D<int> p2, const Point3D<int> p3);

    Point3D<int> &getP1() { return _p1; };
    Point3D<int> &getP2() { return _p2; };
    Point3D<int> &getP3() { return _p3; };
};

#endif
