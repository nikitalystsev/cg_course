#ifndef LIGHT_H
#define LIGHT_H

#include "point/point3D.h"
#include "vector/vector3D.h"

// класс точечного источника освещения
class Light
{
private:
    // положение точечного источника в пространстве
    Point3D<int> _position;
    // интенсивность точечного источника
    double _I_0;
    // коэффициент диффузного отражения
    double _K_d;
    // мощность рассеянного освещения пока что не берем

public:
    Light();
    Light(const Point3D<int> &position, double I_0, double K_d);

    Vector3D<double> caclDirectionVector(const Point3D<double> &vertex);

    double caclIntensityAtVertex(const Vector3D<double> &direction,
                                 const Vector3D<double> &normal);

    Point3D<int> getPosition() const;
    void setPosition(const Point3D<int> &position);
};

#endif // LIGHT_H
