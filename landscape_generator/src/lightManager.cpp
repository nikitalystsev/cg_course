#include "../inc/lightManager.h"

Vector3D<double> LightManager::caclDirectionVector(const Light &light, const Point3D<double> &vertex)
{
    Point3D<int> currPosition(light.getPosition());

    // вектор от точки до источника
    double xDirection = currPosition.getX() - vertex.getX();
    double yDirection = currPosition.getY() - vertex.getY();
    double zDirection = currPosition.getZ() - vertex.getZ();

    return Vector3D<double>(xDirection, yDirection, zDirection);
}

double LightManager::caclIntensityAtVertex(const Light &light,
                                           const Vector3D<double> &direction,
                                           const Vector3D<double> &normal)
{
    double _scalar_product = Vector3D<double>::scalar_product(direction, normal);
    // согласно с формулой для модели освещения Ламберта
    double I = light.getI_0() * light.getK_d() * std::max(0.0, _scalar_product);

    return I;
}
