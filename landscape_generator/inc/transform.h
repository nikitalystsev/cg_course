#ifndef TRANSFORM_H
#define TRANSFORM_H

#define _USE_MATH_DEFINES
#include "point.h"
#include <cmath>

template <typename T>
double toRadians(const T angle)
{
    return angle * (M_PI / 180);
}

template <typename T>
void rotateByX(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_y = point.getY();

    point.setY(point.getY() * cos_angle + point.getZ() * sin_angle);
    //    point.z(-tmp_y * sin_angle + point.z() * cos_angle); // не нужно ее изменение
}

template <typename T>
void rotateByY(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getZ() * sin_angle);
    point.setZ(-tmp_x * sin_angle + point.getZ() * cos_angle);
}

template <typename T>
void rotateByZ(Point3D<T> &point, const double angle)
{
    double cos_angle = cos(toRadians<double>(angle));
    double sin_angle = sin(toRadians<double>(angle));

    double tmp_x = point.getX();

    point.setX(point.getX() * cos_angle + point.getY() * sin_angle);
    point.setY(-tmp_x * sin_angle + point.getY() * cos_angle);
}

template <typename T>
void pointToIsometric(Point3D<T> &point)
{
    rotateByZ(point, 45);
    rotateByX(point, -60);
}

template <typename T>
void swapPoints(Point3D<T> &p1, Point3D<T> &p2)
{
    Point3D<T> tmp(p1);
    p1 = p2;
    p2 = tmp;
}

template <typename T>
void sortByIncreaseY(Point3D<T> &p1, Point3D<T> &p2, Point3D<T> &p3)
{
    // Упорядочиваем точки p1(x1, y1),
    // p2(x2, y2), p3(x3, y3)
    if (p2.getY() < p1.getY())
        swapPoints(p1, p2);
    // точки p1, p2 упорядочены
    if (p3.getY() < p1.getY())
        swapPoints(p1, p3);
    // точки p1, p3 упорядочены
    // теперь p1 самая верхняя
    // осталось упорядочить p2 и p3
    if (p2.getY() > p3.getY())
        swapPoints(p2, p3);
}

#endif
