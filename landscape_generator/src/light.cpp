#include "../inc/light.h"

Light::Light() :
    // пока что случайные значения для последних двух аргументов
    Light(Point3D<int>(0, 0, 0), 1, 0.5)
{
}

Light::Light(const Point3D<int> &position, double I_0, double K_d) :
    _position(position), _I_0(I_0), _K_d(K_d)
{
}

Light::Light(const Light &other)
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;
}

Light::Light(Light &&other) noexcept
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;
}

Light &Light::operator=(const Light &other)
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;

    return *this;
}

Light &Light::operator=(Light &&other) noexcept
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;

    return *this;
}

Point3D<int> Light::getPosition() const
{
    return Point3D<int>(this->_position);
}

void Light::setPosition(const Point3D<int> &position)
{
    this->_position = position;
}
