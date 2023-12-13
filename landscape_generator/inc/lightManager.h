#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

#include "light.h"
#include "vector/vector3D.h"

class LightManager
{
public:
    static Vector3D<double> caclDirectionVector(const Light &light, const Point3D<double> &vertex);
    static double caclIntensityAtVertex(const Light &light, const Vector3D<double> &direction, const Vector3D<double> &normal);
};

#endif // __LIGHT_MANAGER_H__
