#include "../inc/plane.h"

Plane::Plane(const Point3D<double> &p1, const Point3D<double> &p2, const Point3D<double> &p3) :
    _p1(p1), _p2(p2), _p3(p3)
{
    this->_calcPlaneCoeff();
    this->_defCircumRect();
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

    this->_A = n * e - s * p;
    this->_B = k * p - m * e;
    this->_C = m * s - k * n;

    this->_D = -(this->_A * this->_p1.getX() + this->_B * this->_p1.getY() + this->_C * this->_p1.getZ());
}

void Plane::_defCircumRect()
{
    this->_pMin.setX(std::min({this->_p1.getX(), this->_p2.getX(), this->_p3.getX()}));
    this->_pMin.setY(std::min({this->_p1.getY(), this->_p2.getY(), this->_p3.getY()}));

    this->_pMax.setX(std::max({this->_p1.getX(), this->_p2.getX(), this->_p3.getX()}));
    this->_pMax.setY(std::max({this->_p1.getY(), this->_p2.getY(), this->_p3.getY()}));
}

double Plane::caclZ(const double x, const double y)
{
    double z = -(this->_A * x + this->_B * y + this->_D) / this->_C;

    return z;
}

Point3D<double> Plane::getP1() const
{
    return Point3D<double>(this->_p1);
}

Point3D<double> Plane::getP2() const
{
    return Point3D<double>(this->_p2);
}

Point3D<double> Plane::getP3() const
{
    return Point3D<double>(this->_p3);
}

Point2D<double> Plane::getPMin() const
{
    return Point2D<double>(this->_pMin);
}

Point2D<double> Plane::getPMax() const
{
    return Point2D<double>(this->_pMax);
}

double Plane::getA() const
{
    return this->_A;
}

double Plane::getB() const
{
    return this->_B;
}

double Plane::getC() const
{
    return this->_C;
}

double Plane::getD() const
{
    return this->_D;
}
