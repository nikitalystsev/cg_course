#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include <iostream>

template <typename T>
class Vector3D
{
private:
    T _x, _y, _z;

public:
    Vector3D();
    Vector3D(const T x, const T y, const T z);
    explicit Vector3D(const Vector3D<T> &other);
    Vector3D(Vector3D<T> &&other) noexcept;
    ~Vector3D();

    template <typename T2>
    explicit Vector3D(const Vector3D<T2> &other);

    Vector3D<T> &operator=(const Vector3D<T> &other);
    template <typename T2>
    Vector3D<T> &operator=(const Vector3D<T2> &other);

    template <typename T2>
    decltype(auto) operator+(const Vector3D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator-(const Vector3D<T2> &other) const;
    template <typename T2>
    decltype(auto) operator*(const T2 &elem) const;
    template <typename T2>
    decltype(auto) operator/(const T2 &elem) const;

    template <typename T2>
    Vector3D<T> &operator+=(const Vector3D<T2> &other);
    template <typename T2>
    Vector3D<T> &operator-=(const Vector3D<T2> &other);
    template <typename T2>
    Vector3D<T> &operator*=(const T2 &num);
    template <typename T2>
    Vector3D<T> &operator/=(const T2 &num);

    void set(const T x, const T y, const T z);

    void setX(const T x);
    void setY(const T y);
    void setZ(const T z);

    T getX() const;
    T getY() const;
    T getZ() const;

    double lenght() const;
    void normalize();

    void print();

public:
    template <typename T1, typename T2>
    static double scalar_product(const Vector3D<T1> &v1, const Vector3D<T2> &v2);
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
Vector3D<T>::Vector3D(const Vector3D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
}

template <typename T>
Vector3D<T>::Vector3D(Vector3D<T> &&other) noexcept
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;
}

template <typename T>
Vector3D<T>::~Vector3D()
{
}

template <typename T>
template <typename T2>
Vector3D<T>::Vector3D(const Vector3D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_z = other.getZ();
}

template <typename T>
Vector3D<T> &Vector3D<T>::operator=(const Vector3D<T> &other)
{
    this->_x = other._x;
    this->_y = other._y;
    this->_z = other._z;

    return *this;
}

template <typename T>
template <typename T2>
Vector3D<T> &Vector3D<T>::operator=(const Vector3D<T2> &other)
{
    this->_x = other.getX();
    this->_y = other.getY();
    this->_z = other.getZ();

    return *this;
}

template <typename T>
template <typename T2>
decltype(auto) Vector3D<T>::operator+(const Vector3D<T2> &other) const
{
    Vector3D<decltype(this->_x + other.getX())> tmp;

    tmp.setX(this->_x + other.getX());
    tmp.setY(this->_y + other.getY());
    tmp.setZ(this->_z + other.getZ());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector3D<T>::operator-(const Vector3D<T2> &other) const
{
    Vector3D<decltype(this->_x - other.getX())> tmp;

    tmp.setX(this->_x - other.getX());
    tmp.setY(this->_y - other.getY());
    tmp.setZ(this->_z - other.getZ());

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector3D<T>::operator*(const T2 &elem) const
{
    Vector3D<decltype(this->_x * elem)> tmp;

    tmp.setX(this->_x * elem);
    tmp.setY(this->_y * elem);
    tmp.setZ(this->_z * elem);

    return tmp;
}

template <typename T>
template <typename T2>
decltype(auto) Vector3D<T>::operator/(const T2 &elem) const
{
    Vector3D<decltype(this->_x / elem)> tmp;

    tmp.setX(this->_x / elem);
    tmp.setY(this->_y / elem);
    tmp.setZ(this->_z / elem);

    return tmp;
}

template <typename T>
template <typename T2>
Vector3D<T> &Vector3D<T>::operator+=(const Vector3D<T2> &other)
{

    this->_x += other.getX();
    this->_y += other.getY();
    this->_z += other.getZ();

    return *this;
}

template <typename T>
template <typename T2>
Vector3D<T> &Vector3D<T>::operator-=(const Vector3D<T2> &other)
{

    this->_x -= other.getX();
    this->_y -= other.getY();
    this->_z -= other.getZ();

    return *this;
}

template <typename T>
template <typename T2>
Vector3D<T> &Vector3D<T>::operator*=(const T2 &num)
{
    this->_x *= num;
    this->_y *= num;
    this->_z *= num;

    return *this;
}

template <typename T>
template <typename T2>
Vector3D<T> &Vector3D<T>::operator/=(const T2 &num)
{
    this->_x /= num;
    this->_y /= num;
    this->_z /= num;

    return *this;
}

template <typename T>
void Vector3D<T>::set(const T x, const T y, const T z)
{
    this->_x = x;
    this->_y = y;
    this->_z = y;
}

template <typename T>
void Vector3D<T>::setX(const T x)
{
    this->_x = x;
}

template <typename T>
void Vector3D<T>::setY(const T y)
{
    this->_y = y;
}

template <typename T>
void Vector3D<T>::setZ(const T z)
{
    this->_z = z;
}

template <typename T>
T Vector3D<T>::getX() const
{
    return this->_x;
}

template <typename T>
T Vector3D<T>::getY() const
{
    return this->_y;
}

template <typename T>
T Vector3D<T>::getZ() const
{
    return this->_z;
}

template <typename T>
double Vector3D<T>::lenght() const
{
    T x_sqr = this->_x * this->_x;
    T y_sqr = this->_y * this->_y;
    T z_sqr = this->_z * this->_z;

    return std::sqrt(x_sqr + y_sqr + z_sqr);
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
void Vector3D<T>::print()
{
    std::cout << "Vector3D(" << _x << ", " << _y << ", " << _z << ")" << std::endl;
}

template <typename T>
template <typename T1, typename T2>
double Vector3D<T>::scalar_product(const Vector3D<T1> &v1, const Vector3D<T2> &v2)
{
    double res = v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ();

    return res;
}

#endif // VECTOR3D_H
