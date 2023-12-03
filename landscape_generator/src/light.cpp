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

Vector3D<double> Light::caclDirectionVector(const Point3D<double> &vertex)
{
    // вектор от точки до источника
    double xDirection = this->_position.getX() - vertex.getX();
    double yDirection = this->_position.getY() - vertex.getY();
    double zDirection = this->_position.getZ() - vertex.getZ();

    return Vector3D<double>(xDirection, yDirection, zDirection);
}

double Light::caclIntensityAtVertex(const Vector3D<double> &direction,
                                    const Vector3D<double> &normal)
{
    // согласно с формулой для модели освещения Ламберта
    double _scalar_product = Vector3D<double>::scalar_product(direction, normal);
    double I = _scalar_product;

    return I;
}

Point3D<int> Light::getPosition() const
{
    return Point3D<int>(this->_position);
}

void Light::setPosition(const Point3D<int> &position)
{
    this->_position = position;
}
