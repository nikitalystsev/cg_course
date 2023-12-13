#ifndef __POINT2D_H__
#define __POINT2D_H__

#include <iostream>

template <typename T>
class Point2D // ака точка на двумерной плоскости, ака пиксель
{
private:
    T _x, _y;

    double _I; // интенсивность

public:
    Point2D();
    Point2D(const T x, const T y);
    explicit Point2D(const Point2D<T> &other);
    Point2D(Point2D<T> &&other) noexcept;
    ~Point2D();

    template <typename T2>
    explicit Point2D(const Point2D<T2> &other);

    Point2D<T> &operator=(const Point2D<T> &other);
    template <typename T2>
    Point2D<T> &operator=(const Point2D<T2> &other);

    template <typename T2>
    decltype(auto) operator+(const Point2D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator-(const Point2D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator*(const T2 elem) const;

    void set(const T x, const T y);

    void setX(const T x);
    void setY(const T y);
    void setI(const double I);

    T getX() const;
    T getY() const;
    double getI() const;

    void print() const;

public:
    static inline int cmpPointsByX(const Point2D<T> &a, const Point2D<T> &b);
    static bool isPointYEqualTo(const Point2D<T> &point, int y);
};

template <typename T>
Point2D<T>::Point2D() :
    Point2D(0, 0)

{
}

template <typename T>
Point2D<T>::Point2D(const T x, const T y) :
    _x(x), _y(y), _I(0)
{
}

template <typename T>
Point2D<T>::Point2D(const Point2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_I = other._I;
}

template <typename T>
Point2D<T>::Point2D(Point2D<T> &&other) noexcept
{
    this->_x = other._x;
    this->_y = other._y;
    this->_I = other._I;
}

template <typename T>
Point2D<T>::~Point2D()
{
}

template <typename T>
template <typename T2>
Point2D<T>::Point2D(const Point2D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_I = other.getI();
}

template <typename T>
Point2D<T> &Point2D<T>::operator=(const Point2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_I = other._I;

    return *this;
}

template <typename T>
template <typename T2>
Point2D<T> &Point2D<T>::operator=(const Point2D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_I = other.getI();

    return *this;
}

template <typename T>
template <typename T2>
decltype(auto) Point2D<T>::operator+(const Point2D<T2> &other) const
{
    Point2D<decltype(this->_x + other.getX())> tmp;

    tmp.setX(this->_x + other.getX());
    tmp.setY(this->_y + other.getY());
    tmp.setI(this->_I + other.getI());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Point2D<T>::operator-(const Point2D<T2> &other) const
{
    Point2D<decltype(this->_x - other.getX())> tmp;

    tmp.setX(this->_x - other.getX());
    tmp.setY(this->_y - other.getY());
    tmp.setI(this->_I - other.getI());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Point2D<T>::operator*(const T2 elem) const
{
    Point2D<decltype(this->_x * elem)> tmp;

    tmp.setX(this->_x * elem);
    tmp.setY(this->_y * elem);

    return tmp;
}

template <typename T>
void Point2D<T>::set(const T x, const T y)
{
    this->_x = x;
    this->_y = y;
}

template <typename T>
void Point2D<T>::setX(const T x)
{
    this->_x = x;
}

template <typename T>
void Point2D<T>::setY(const T y)
{
    this->_y = y;
}

template <typename T>
void Point2D<T>::setI(const double I)
{
    this->_I = I;
}

template <typename T>
T Point2D<T>::getX() const
{
    return this->_x;
}

template <typename T>
T Point2D<T>::getY() const
{
    return this->_y;
}

template <typename T>
double Point2D<T>::getI() const
{
    return this->_I;
}

template <typename T>
void Point2D<T>::print() const
{
    std::cout << "Point2D(" << this->_x << "," << this->_y << ")" << std::endl;
}

template <typename T>
inline int Point2D<T>::cmpPointsByX(const Point2D<T> &a, const Point2D<T> &b)
{
    return a.getX() < b.getX();
}

template <typename T>
bool Point2D<T>::isPointYEqualTo(const Point2D<T> &point, int y)
{
    return point.getY() == y;
}

#endif // __POINT2D_H__
