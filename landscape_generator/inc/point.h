#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

using namespace std;

template <typename T>
class Point3D
{
private:
    T _x, _y, _z;

public:
    Point3D();
    Point3D(const T x, const T y, const T z);
    explicit Point3D(const Point3D<T> &other);
    ~Point3D();

    void setPoint3D(const T x, const T y, const T z);

    void setX(const T x);
    void setY(const T y);
    void setZ(const T z);

    T getX() const;
    T getY() const;
    T getZ() const;

    void printPoint() const;
};

template <typename T>
Point3D<T>::Point3D() :
    Point3D(0, 0, 0)

{
}

template <typename T>
Point3D<T>::Point3D(const T x, const T y, const T z) :
    _x(x), _y(y), _z(z)
{
}

template <typename T>
Point3D<T>::Point3D(const Point3D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
}

template <typename T>
Point3D<T>::~Point3D()
{
}

template <typename T>
void Point3D<T>::setPoint3D(const T x, const T y, const T z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

template <typename T>
void Point3D<T>::setX(const T x)
{
    this->_x = x;
}

template <typename T>
void Point3D<T>::setY(const T y)
{
    this->_y = y;
}

template <typename T>
void Point3D<T>::setZ(const T z)
{
    this->_z = z;
}

template <typename T>
T Point3D<T>::getX() const
{
    return this->_x;
}

template <typename T>
T Point3D<T>::getY() const
{
    return this->_y;
}

template <typename T>
T Point3D<T>::getZ() const
{
    return this->_z;
}

template <typename T>
void Point3D<T>::printPoint() const
{
    cout << "(" << this->_x << "," << this->_y << "," << this->_z << ")" << endl;
}

template <typename T>
double toRadians(const T angle)
{
    return angle * (M_PI / 180);
}

template <typename T>
void rotateByX(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_y = point.getY();

    point.setY(point.getY() * cos_angle + point.getZ() * sin_angle);
    //    point.z(-tmp_y * sin_angle + point.z() * cos_angle); // не нужно ее изменение
}

template <typename T>
void rotateByY(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getZ() * sin_angle);
    point.setZ(-tmp_x * sin_angle + point.getZ() * cos_angle);
}

template <typename T>
void rotateByZ(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getY() * sin_angle);
    point.setY(-tmp_x * sin_angle + point.getY() * cos_angle);
}

template <typename T>
void pointToIsometric(Point3D<T> &point)
{
    rotateByZ(point, 45);
    rotateByX(point, -60);
}

#endif // POINT_H
