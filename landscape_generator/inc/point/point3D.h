#ifndef __POINT3D_H__
#define __POINT3D_H__

#include <iostream>

template <typename T>
class Point3D
{
private:
    T _x, _y, _z;

    double _I; // интенсивность

public:
    Point3D();
    Point3D(const T x, const T y, const T z);
    explicit Point3D(const Point3D<T> &other);
    Point3D(Point3D<T> &&other) noexcept;
    ~Point3D();

    template <typename T2>
    explicit Point3D(const Point3D<T2> &other);

    Point3D<T> &operator=(const Point3D<T> &other);
    template <typename T2>
    Point3D<T> &operator=(const Point3D<T2> &other);

    template <typename T2>
    decltype(auto) operator+(const Point3D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator-(const Point3D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator*(const T2 elem) const;

    void set(const T x, const T y, const T z);

    void setX(const T x);
    void setY(const T y);
    void setZ(const T z);
    void setI(const double I);

    T getX() const;
    T getY() const;
    T getZ() const;
    double getI() const;

    void print() const;

public:
    static inline int cmpPointsByX(const Point3D<T> &a, const Point3D<T> &b);
};

template <typename T>
Point3D<T>::Point3D() :
    Point3D(0, 0, 0)

{
}

template <typename T>
Point3D<T>::Point3D(const T x, const T y, const T z) :
    _x(x), _y(y), _z(z), _I(0)
{
}

template <typename T>
Point3D<T>::Point3D(const Point3D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
    this->_I = other._I;
}

template <typename T>
Point3D<T>::Point3D(Point3D<T> &&other) noexcept
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
    this->_I = other._I;
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
    this->_I = other.getI();
}

template <typename T>
Point3D<T> &Point3D<T>::operator=(const Point3D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
    this->_I = other._I;

    return *this;
}

template <typename T>
template <typename T2>
Point3D<T> &Point3D<T>::operator=(const Point3D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_z = other.getZ();
    this->_I = other.getI();

    return *this;
}

template <typename T>
template <typename T2>
decltype(auto) Point3D<T>::operator+(const Point3D<T2> &other) const
{
    Point3D<decltype(this->_x + other.getX())> tmp;

    tmp.setX(this->_x + other.getX());
    tmp.setY(this->_y + other.getY());
    tmp.setZ(this->_z + other.getZ());
    tmp.setI(this->_I + other.getI());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Point3D<T>::operator-(const Point3D<T2> &other) const
{
    Point3D<decltype(this->_x - other.getX())> tmp;

    tmp.setX(this->_x - other.getX());
    tmp.setY(this->_y - other.getY());
    tmp.setZ(this->_z - other.getZ());
    tmp.setI(this->_I - other.getI());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Point3D<T>::operator*(const T2 elem) const
{
    Point3D<decltype(this->_x * elem)> tmp;

    tmp.setX(this->_x * elem);
    tmp.setY(this->_y * elem);
    tmp.setZ(this->_z * elem);
    tmp.setI(this->_I * elem);

    return tmp;
}

template <typename T>
void Point3D<T>::set(const T x, const T y, const T z)
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
void Point3D<T>::setI(const double I)
{
    this->_I = I;
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
double Point3D<T>::getI() const
{
    return this->_I;
}

template <typename T>
void Point3D<T>::print() const
{
    std::cout << "Point3D(" << this->_x << "," << this->_y << "," << this->_z << ")" << std::endl;
}

template <typename T>
inline int Point3D<T>::cmpPointsByX(const Point3D<T> &a, const Point3D<T> &b)
{
    return a.getX() < b.getX();
}

#endif // __POINT3D_H__
