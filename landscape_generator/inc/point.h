#ifndef POINT_H
#define POINT_H

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
    Point3D(Point3D<T> &&other) noexcept;
    ~Point3D();

    template <typename T2>
    explicit Point3D(const Point3D<T2> &other);

    void setPoint3D(const T x, const T y, const T z);

    void setX(const T x);
    void setY(const T y);
    void setZ(const T z);

    T getX() const;
    T getY() const;
    T getZ() const;

    void printPoint3D() const;
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
Point3D<T>::Point3D(Point3D<T> &&other) noexcept
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
template <typename T2>
Point3D<T>::Point3D(const Point3D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_z = other.getZ();
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
void Point3D<T>::printPoint3D() const
{
    cout << "(" << this->_x << "," << this->_y << "," << this->_z << ")" << endl;
}

#endif // POINT_H
