#include "../inc/lightManager.h"

QVector3D LightManager::caclDirectionVector(const Light &light, const QVector3D &vertex)
{
    QVector3D currPosition(light.getPosition());

    // вектор от точки до источника
    double xDirection = currPosition.x() - vertex.x();
    double yDirection = currPosition.y() - vertex.y();
    double zDirection = currPosition.z() - vertex.z();

    return QVector3D(xDirection, yDirection, zDirection);
}

double LightManager::caclIntensityAtVertex(const Light &light,
                                           const QVector3D &direction,
                                           const QVector3D &normal)
{
    double _scalar_product = QVector3D::dotProduct(direction, normal);
    // согласно с формулой для модели освещения Ламберта
    double I = light.getI_0() * light.getK_d() * std::max(0.0, _scalar_product);

    return I;
}
