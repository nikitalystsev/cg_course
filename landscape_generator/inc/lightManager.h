#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

#include "light.h"

#include <QVector3D>

class LightManager
{
public:
    static QVector3D caclDirectionVector(const Light &light, const QVector3D &vertex);
    static double caclIntensityAtVertex(const Light &light, const QVector3D &direction, const QVector3D &normal);
};

#endif // __LIGHT_MANAGER_H__
