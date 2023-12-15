#include "../inc/transform.h"

double Transform::toRadians(const double angle)
{
    return angle * (M_PI / 180);
}

void Transform::rotateByX(QVector3D &point, const double angle)
{
    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_y = point.y();

    point.setY(point.y() * cos_angle + point.z() * sin_angle);
    point.setZ(-tmp_y * sin_angle + point.z() * cos_angle);
}

void Transform::rotateByY(QVector3D &point, const double angle)
{
    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(point.x() * cos_angle - point.z() * sin_angle);
    point.setZ(tmp_x * sin_angle + point.z() * cos_angle);
}

void Transform::rotateByZ(QVector3D &point, const double angle)
{
    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(point.x() * cos_angle + point.y() * sin_angle);
    point.setY(-tmp_x * sin_angle + point.y() * cos_angle);
}

void Transform::rotate(QVector3D &point, const rotate_t &rotate)
{
    rotateByX(point, rotate.xAngle);
    rotateByY(point, rotate.yAngle);
    rotateByZ(point, rotate.zAngle);
}

void Transform::move(QVector3D &point, const move_t &move)
{
    point.setX(point.x() + move.dx);
    point.setY(point.y() + move.dy);
    point.setZ(point.z() + move.dz);
}
