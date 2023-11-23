#ifndef __POINT2D_H__
#define __POINT2D_H__

#include <iostream>

template <typename T>
class Point2D
{
private:
    T _x, _y;

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

    T getX() const;
    T getY() const;

    void print() const;
};

template <typename T>
Point2D<T>::Point2D() :
    Point2D(0, 0)

{
}

template <typename T>
Point2D<T>::Point2D(const T x, const T y) :
    _x(x), _y(y)
{
}

template <typename T>
Point2D<T>::Point2D(const Point2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
}

template <typename T>
Point2D<T>::Point2D(Point2D<T> &&other) noexcept
{
    this->_x = other._x;
    this->_y = other._y;
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
}

template <typename T>
Point2D<T> &Point2D<T>::operator=(const Point2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;

    return *this;
}

template <typename T>
template <typename T2>
Point2D<T> &Point2D<T>::operator=(const Point2D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();

    return *this;
}

template <typename T>
template <typename T2>
decltype(auto) Point2D<T>::operator+(const Point2D<T2> &other) const
{
    Point2D<decltype(this->_x + other.getX())> tmp;

    tmp.setX(this->_x + other.getX());
    tmp.setY(this->_y + other.getY());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Point2D<T>::operator-(const Point2D<T2> &other) const
{
    Point2D<decltype(this->_x - other.getX())> tmp;

    tmp.setX(this->_x - other.getX());
    tmp.setY(this->_y - other.getY());

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
void Point2D<T>::print() const
{
    std::cout << "Point2D(" << this->_x << "," << this->_y << ")" << std::endl;
}

#endif // __POINT2D_H__