#ifndef TRANSFORM_H
#define TRANSFORM_H

#define _USE_MATH_DEFINES
#include "point/point3D.h"
#include <cmath>

// класс трансформаций для точки
class Transform
{
public:
    template <typename T>
    static double toRadians(const T angle);

    template <typename T>
    static void rotateByX(Point3D<T> &point, const double angle);

    template <typename T>
    static void rotateByY(Point3D<T> &point, const double angle);

    template <typename T>
    static void rotateByZ(Point3D<T> &point, const double angle);

    template <typename T>
    static void pointToIsometric(Point3D<T> &point);
};

template <typename T>
double Transform::toRadians(const T angle)
{
    return angle * (M_PI / 180);
}

template <typename T>
void Transform::rotateByX(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_y = point.getY();

    point.setY(point.getY() * cos_angle + point.getZ() * sin_angle);
    point.setZ(-tmp_y * sin_angle + point.getZ() * cos_angle);
}

template <typename T>
void Transform::rotateByY(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getZ() * sin_angle);
    point.setZ(-tmp_x * sin_angle + point.getZ() * cos_angle);
}

template <typename T>
void Transform::rotateByZ(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getY() * sin_angle);
    point.setY(-tmp_x * sin_angle + point.getY() * cos_angle);
}

template <typename T>
void Transform::pointToIsometric(Point3D<T> &point)
{
    rotateByZ(point, 45);
    rotateByX(point, -60);
    // rotateByX(point, -50);
}

#endif
