#include "../inc/plane.h"

Plane::Plane(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3) :
    _p1(p1), _p2(p2), _p3(p3)
{
    this->_calcPlaneCoeff();
    this->_defCircumRect();
}

void Plane::_calcPlaneCoeff()
{
    // плоскость, заданная тремя точками _p1, _p2, _p3
    double m = this->_p2.x() - this->_p1.x();
    double n = this->_p2.y() - this->_p1.y();
    double p = this->_p2.z() - this->_p1.z();

    double k = this->_p3.x() - this->_p1.x();
    double s = this->_p3.y() - this->_p1.y();
    double e = this->_p3.z() - this->_p1.z();

    this->_A = n * e - s * p;
    this->_B = k * p - m * e;
    this->_C = m * s - k * n;

    this->_D = -(this->_A * this->_p1.x() + this->_B * this->_p1.y() + this->_C * this->_p1.z());
}

void Plane::_defCircumRect()
{
    this->_pMin.setX(std::min({this->_p1.x(), this->_p2.x(), this->_p3.x()}));
    this->_pMin.setY(std::min({this->_p1.y(), this->_p2.y(), this->_p3.y()}));

    this->_pMax.setX(std::max({this->_p1.x(), this->_p2.x(), this->_p3.x()}));
    this->_pMax.setY(std::max({this->_p1.y(), this->_p2.y(), this->_p3.y()}));
}

double Plane::caclZ(const double x, const double y) const
{
    double z = -(this->_A * x + this->_B * y + this->_D) / this->_C;

    return z;
}

QVector3D Plane::getP1() const
{
    return QVector3D(this->_p1);
}

QVector3D Plane::getP2() const
{
    return QVector3D(this->_p2);
}

QVector3D Plane::getP3() const
{
    return QVector3D(this->_p3);
}

QVector2D Plane::getPMin() const
{
    return QVector2D(this->_pMin);
}

QVector2D Plane::getPMax() const
{
    return QVector2D(this->_pMax);
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
