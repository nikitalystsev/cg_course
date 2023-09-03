#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

template <typename T>
class Vector3D
{
public:
    Vector3D();
    Vector3D(T x, T y, T z);
    ~Vector3D();

    void set(const T x, const T y, const T z);

    void set_x(T x);
    void set_y(T y);
    void set_z(T z);

    T get_x() const;
    T get_y() const;
    T get_z() const;

    Vector3D<T> operator+(const Vector3D<T> &other) const;
    Vector3D<T> &operator+=(const Vector3D<T> &other);
    Vector3D<T> operator-(const Vector3D<T> &other) const;
    Vector3D<T> &operator-=(const Vector3D<T> &other);
    Vector3D<T> operator*(const T &num) const;
    Vector3D<T> &operator*=(const T &num);
    Vector3D<T> operator/(const T &num) const;
    Vector3D<T> &operator/=(const T &num);

    double lenght() const;
    template <typename T2>
    double scalar_product(const Vector3D<T2> &other);
    void normalize();

    void print();

private:
    T _x, _y, _z;
};

template <typename T>
Vector3D<T>::Vector3D() :
    _x(0), _y(0), _z(0)
{
}

template <typename T>
Vector3D<T>::Vector3D(T x, T y, T z) :
    _x(x), _y(y), _z(z)
{
}

template <typename T>
Vector3D<T>::~Vector3D()
{
}

template <typename T>
void Vector3D<T>::set(const T x, const T y, const T z)
{
    this->_x = x;
    this->_y = y;
    this->_z = y;
}

template <typename T>
void Vector3D<T>::set_x(T x)
{
    this->_x = x;
}

template <typename T>
void Vector3D<T>::set_y(T y)
{
    this->_y = y;
}

template <typename T>
void Vector3D<T>::set_z(T z)
{
    this->_z = z;
}

template <typename T>
T Vector3D<T>::get_x() const
{
    return this->_x;
}

template <typename T>
T Vector3D<T>::get_y() const
{
    return this->_y;
}

template <typename T>
T Vector3D<T>::get_z() const
{
    return this->_z;
}

template <typename T>
void Vector3D<T>::print()
{
    std::cout << "Vector(" << _x << ", " << _y << ", " << _z << ")" << std::endl;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator+(const Vector3D<T> &other) const
{
    Vector3D<T> new_vector;

    T new_x = this->_x + other._x;
    T new_y = this->_y + other._y;
    T new_z = this->_z + other._z;

    new_vector.set(new_x, new_y, new_z);

    return new_vector;
}

template <typename T>
Vector3D<T> &Vector3D<T>::operator+=(const Vector3D<T> &other)
{

    this->_x += other._x;
    this->_y += other._y;
    this->_z += other._z;

    return *this;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator-(const Vector3D<T> &other) const
{
    Vector3D<T> new_vector;

    T new_x = this->_x - other._x;
    T new_y = this->_y - other._y;
    T new_z = this->_z - other._z;

    new_vector.set(new_x, new_y, new_z);

    return new_vector;
}

template <typename T>
Vector3D<T> &Vector3D<T>::operator-=(const Vector3D<T> &other)
{

    this->_x -= other._x;
    this->_y -= other._y;
    this->_z -= other._z;

    return *this;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator*(const T &num) const
{
    Vector3D<T> new_vector;

    T new_x = this->_x * num;
    T new_y = this->_y * num;
    T new_z = this->_z * num;

    new_vector.set(new_x, new_y, new_z);

    return new_vector;
}

template <typename T>
Vector3D<T> &Vector3D<T>::operator*=(const T &num)
{
    this->_x *= num;
    this->_y *= num;
    this->_z *= num;

    return *this;
}

template <typename T>
Vector3D<T> Vector3D<T>::operator/(const T &num) const
{
    Vector3D<T> new_vector;

    T new_x = this->_x / num;
    T new_y = this->_y / num;
    T new_z = this->_z / num;

    new_vector.set(new_x, new_y, new_z);

    return new_vector;
}

template <typename T>
Vector3D<T> &Vector3D<T>::operator/=(const T &num)
{
    this->_x /= num;
    this->_y /= num;
    this->_z /= num;

    return *this;
}

template <typename T>
template <typename T2>
double Vector3D<T>::scalar_product(const Vector3D<T2> &other)
{
    return this->_x * other._x + this->_y * other._y + this->_z * other._z;
}

template <typename T>
void Vector3D<T>::normalize()
{
    double lenght_vector = this->lenght();

    this->_x /= lenght_vector;
    this->_y /= lenght_vector;
    this->_z /= lenght_vector;
}

template <typename T>
double Vector3D<T>::lenght() const
{
    T x_sqr = this->_x * this->_x;
    T y_sqr = this->_y * this->_y;
    T z_sqr = this->_z * this->_z;

    return std::sqrt(x_sqr + y_sqr + z_sqr);
}

#endif // VECTOR_H
