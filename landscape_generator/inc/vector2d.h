#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>

template <typename T>
class Vector2D
{
public:
    Vector2D();
    Vector2D(T x, T y);
    ~Vector2D();

    Vector2D<T> &operator=(const Vector2D<T> &matrix);
    //    Vector2D<T> &operator=(Vector2D<T> &&matrix) noexcept;

    void set(const T x, const T y);

    void x(const T x);
    void y(const T y);

    T x() const;
    T y() const;

    Vector2D<T> operator+(const Vector2D<T> &other) const;
    Vector2D<T> &operator+=(const Vector2D<T> &other);
    Vector2D<T> operator-(const Vector2D<T> &other) const;
    Vector2D<T> &operator-=(const Vector2D<T> &other);
    Vector2D<T> operator*(const T &num) const;
    Vector2D<T> &operator*=(const T &num);
    Vector2D<T> operator/(const T &num) const;
    Vector2D<T> &operator/=(const T &num);

    double lenght() const;
    template <typename T2>
    double scalar_product(const Vector2D<T2> &other) const;
    void normalize();

    void print();

private:
    T _x, _y;
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
Vector2D<T>::~Vector2D()
{
}

// операторы присваивания
template <typename T>
Vector2D<T> &Vector2D<T>::operator=(const Vector2D<T> &other)
{
    //    std::cout << "call assignment copy operator" << std::endl;

    this->_x = other._x;
    this->_y = other.y;

    return *this;
}

// template <typename T>
// Matrix<T> &Matrix<T>::operator=(Matrix<T> &&matrix) noexcept
//{
//     //    std::cout << "call assignment move operator" << std::endl;

//    this->_data = matrix._data;
//    this->_rows = matrix._rows;
//    this->_cols = matrix._cols;

//    return *this;
//}

template <typename T>
void Vector2D<T>::set(const T x, const T y)
{
    this->_x = x;
    this->_y = y;
}

template <typename T>
void Vector2D<T>::x(const T x)
{
    this->_x = x;
}

template <typename T>
void Vector2D<T>::y(const T y)
{
    this->_y = y;
}

template <typename T>
T Vector2D<T>::x() const
{
    return this->_x;
}

template <typename T>
T Vector2D<T>::y() const
{
    return this->_y;
}

template <typename T>
void Vector2D<T>::print()
{
    std::cout << "Vector(" << _x << ", " << _y << ")" << std::endl;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+(const Vector2D<T> &other) const
{
    Vector2D<T> new_vector;

    T new_x = this->_x + other._x;
    T new_y = this->_y + other._y;

    new_vector.set(new_x, new_y);

    return new_vector;
}

template <typename T>
Vector2D<T> &Vector2D<T>::operator+=(const Vector2D<T> &other)
{

    this->_x += other._x;
    this->_y += other._y;

    return *this;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator-(const Vector2D<T> &other) const
{
    Vector2D<T> new_vector;

    T new_x = this->_x - other._x;
    T new_y = this->_y - other._y;

    new_vector.set(new_x, new_y);

    return new_vector;
}

template <typename T>
Vector2D<T> &Vector2D<T>::operator-=(const Vector2D<T> &other)
{

    this->_x -= other._x;
    this->_y -= other._y;

    return *this;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator*(const T &num) const
{
    Vector2D<T> new_vector;

    T new_x = this->_x * num;
    T new_y = this->_y * num;

    new_vector.set(new_x, new_y);

    return new_vector;
}

template <typename T>
Vector2D<T> &Vector2D<T>::operator*=(const T &num)
{
    this->_x *= num;
    this->_y *= num;

    return *this;
}

template <typename T>
Vector2D<T> Vector2D<T>::operator/(const T &num) const
{
    Vector2D<T> new_vector;

    T new_x = this->_x / num;
    T new_y = this->_y / num;

    new_vector.set(new_x, new_y);

    return new_vector;
}

template <typename T>
Vector2D<T> &Vector2D<T>::operator/=(const T &num)
{
    this->_x /= num;
    this->_y /= num;

    return *this;
}

template <typename T>
template <typename T2>
double Vector2D<T>::scalar_product(const Vector2D<T2> &other) const
{
    return this->_x * other._x + this->_y * other._y;
}

template <typename T>
void Vector2D<T>::normalize()
{
    double lenght_vector = this->lenght();

    this->_x /= lenght_vector;
    this->_y /= lenght_vector;
}

template <typename T>
double Vector2D<T>::lenght() const
{
    T x_sqr = this->_x * this->_x;
    T y_sqr = this->_y * this->_y;

    return std::sqrt(x_sqr + y_sqr);
}

#endif // VECTOR2D_H
