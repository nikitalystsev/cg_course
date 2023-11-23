#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>

template <typename T>
class Vector2D
{
private:
    T _x, _y;

public:
    Vector2D();
    Vector2D(const T x, const T y);
    explicit Vector2D(const Vector2D<T> &other);
    Vector2D(Vector2D<T> &&other) noexcept;
    ~Vector2D();

    template <typename T2>
    explicit Vector2D(const Vector2D<T2> &other);

    Vector2D<T> &operator=(const Vector2D<T> &other);
    template <typename T2>
    Vector2D<T> &operator=(const Vector2D<T2> &other);

    template <typename T2>
    decltype(auto) operator+(const Vector2D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator-(const Vector2D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator*(const T2 &elem) const;
    template <typename T2>
    decltype(auto) operator/(const T2 &elem) const;

    template <typename T2>
    Vector2D<T> &operator+=(const Vector2D<T2> &other);
    template <typename T2>
    Vector2D<T> &operator-=(const Vector2D<T2> &other);
    template <typename T2>
    Vector2D<T> &operator*=(const T2 &num);
    template <typename T2>
    Vector2D<T> &operator/=(const T2 &num);

    void set(const T x, const T y);

    void setX(const T x);
    void setY(const T y);

    T getX() const;
    T getY() const;

    double lenght() const;
    void normalize();

    void print();

public:
    template <typename T1, typename T2>
    static double scalar_product(const Vector2D<T1> &v1, const Vector2D<T2> &v2);
};

template <typename T>
Vector2D<T>::Vector2D() :
    _x(0), _y(0)
{
}

template <typename T>
Vector2D<T>::Vector2D(T x, T y) :
    _x(x), _y(y)
{
}

template <typename T>
Vector2D<T>::Vector2D(const Vector2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
}

template <typename T>
Vector2D<T>::Vector2D(Vector2D<T> &&other) noexcept
{
    this->_x = other._x;
    this->_y = other._y;
}

template <typename T>
Vector2D<T>::~Vector2D()
{
}

template <typename T>
template <typename T2>
Vector2D<T>::Vector2D(const Vector2D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
}

template <typename T>
Vector2D<T> &Vector2D<T>::operator=(const Vector2D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;

    return *this;
}

template <typename T>
template <typename T2>
Vector2D<T> &Vector2D<T>::operator=(const Vector2D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();

    return *this;
}

template <typename T>
template <typename T2>
decltype(auto) Vector2D<T>::operator+(const Vector2D<T2> &other) const
{
    Vector2D<decltype(this->_x + other.getX())> tmp;

    tmp.setX(this->_x + other.getX());
    tmp.setY(this->_y + other.getY());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector2D<T>::operator-(const Vector2D<T2> &other) const
{
    Vector2D<decltype(this->_x - other.getX())> tmp;

    tmp.setX(this->_x - other.getX());
    tmp.setY(this->_y - other.getY());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector2D<T>::operator*(const T2 &elem) const
{
    Vector2D<decltype(this->_x * elem)> tmp;

    tmp.setX(this->_x * elem);
    tmp.setY(this->_y * elem);

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector2D<T>::operator/(const T2 &elem) const
{
    Vector2D<decltype(this->_x / elem)> tmp;

    tmp.setX(this->_x / elem);
    tmp.setY(this->_y / elem);

    return tmp;
}

template <typename T>
template <typename T2>
Vector2D<T> &Vector2D<T>::operator+=(const Vector2D<T2> &other)
{

    this->_x += other.getX();
    this->_y += other.getY();

    return *this;
}

template <typename T>
template <typename T2>
Vector2D<T> &Vector2D<T>::operator-=(const Vector2D<T2> &other)
{

    this->_x -= other.getX();
    this->_y -= other.getY();

    return *this;
}

template <typename T>
template <typename T2>
Vector2D<T> &Vector2D<T>::operator*=(const T2 &num)
{
    this->_x *= num;
    this->_y *= num;
    return *this;
}

template <typename T>
template <typename T2>
Vector2D<T> &Vector2D<T>::operator/=(const T2 &num)
{
    this->_x /= num;
    this->_y /= num;

    return *this;
}

template <typename T>
void Vector2D<T>::set(const T x, const T y)
{
    this->_x = x;
    this->_y = y;
}

template <typename T>
void Vector2D<T>::setX(const T x)
{
    this->_x = x;
}

template <typename T>
void Vector2D<T>::setY(const T y)
{
    this->_y = y;
}

template <typename T>
T Vector2D<T>::getX() const
{
    return this->_x;
}

template <typename T>
T Vector2D<T>::getY() const
{
    return this->_y;
}

template <typename T>
double Vector2D<T>::lenght() const
{
    T x_sqr = this->_x * this->_x;
    T y_sqr = this->_y * this->_y;

    return std::sqrt(x_sqr + y_sqr);
}

template <typename T>
void Vector2D<T>::normalize()
{
    double lenght_vector = this->lenght();

    this->_x /= lenght_vector;
    this->_y /= lenght_vector;
}

template <typename T>
void Vector2D<T>::print()
{
    std::cout << "Vector2D(" << _x << ", " << _y << ")" << std::endl;
}

template <typename T>
template <typename T1, typename T2>
double Vector2D<T>::scalar_product(const Vector2D<T1> &v1, const Vector2D<T2> &v2)
{
    double res = v1.getX() * v2.getX() + v1.getY() * v2.getY();

    return res;
}
#endif // VECTOR2D_H
