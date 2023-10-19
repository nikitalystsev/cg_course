#include "../inc/plane.h"

Plane::Plane(const Point3D<int> p1, const Point3D<int> p2, const Point3D<int> p3) :
    _p1(p1), _p2(p2), _p3(p3)
{
    this->_calcPlaneCoeff();
}

void Plane::_calcPlaneCoeff()
{
    // плоскость, заданная тремя точками _p1, _p2, _p3
    double m = this->_p2.getX() - this->_p1.getX();
    double n = this->_p2.getY() - this->_p1.getY();
    double p = this->_p2.getZ() - this->_p1.getZ();

    double k = this->_p3.getX() - this->_p1.getX();
    double s = this->_p3.getY() - this->_p1.getY();
    double e = this->_p3.getZ() - this->_p1.getZ();

    this->A = n * e - s * p;
    this->B = k * p - m * e;
    this->C = m * s - k * n;
    this->D = -(this->A * this->_p1.getX() + this->B * this->_p1.getY() + this->C * this->_p1.getZ());
}
