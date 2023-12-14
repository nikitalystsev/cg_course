#ifndef TRANSFORM_H
#define TRANSFORM_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <QVector3D>

// класс трансформаций для точки
class Transform
{
public:
    static double toRadians(const double angle)
    {
        return angle * (M_PI / 180);
    }

    static void rotateByX(QVector3D &point, const double angle)
    {
        double cos_angle = cos(toRadians(angle));
        double sin_angle = sin(toRadians(angle));

        double tmp_y = point.y();

        point.setY(point.y() * cos_angle + point.z() * sin_angle);
        point.setZ(-tmp_y * sin_angle + point.z() * cos_angle);
    }

    static void rotateByY(QVector3D &point, const double angle)
    {
        double cos_angle = cos(toRadians(angle));
        double sin_angle = sin(toRadians(angle));

        double tmp_x = point.x();

        point.setX(point.x() * cos_angle - point.z() * sin_angle);
        point.setZ(tmp_x * sin_angle + point.z() * cos_angle);
    }

    static void rotateByZ(QVector3D &point, const double angle)
    {
        double cos_angle = cos(toRadians(angle));
        double sin_angle = sin(toRadians(angle));

        double tmp_x = point.x();

        point.setX(point.x() * cos_angle + point.y() * sin_angle);
        point.setY(-tmp_x * sin_angle + point.y() * cos_angle);
    }
};

#endif
