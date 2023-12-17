#include "../inc/transform.h"

double Transform::toRadians(const double angle)
{
    return angle * (M_PI / 180);
}

void Transform::rotateByX(QVector3D &point, const double angle, QVector3D &centerRotate)
{
    double Yc = centerRotate.y();
    double Zc = centerRotate.z();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_y = point.y();

    point.setY(Yc + (point.y() - Yc) * cos_angle + (point.z() - Zc) * sin_angle);
    point.setZ(Zc + (Yc - tmp_y) * sin_angle + (point.z() - Zc) * cos_angle);
}

void Transform::rotateByY(QVector3D &point, const double angle, QVector3D &centerRotate)
{
    double Xc = centerRotate.x();
    double Zc = centerRotate.z();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(Xc + (point.x() - Xc) * cos_angle - (point.z() - Zc) * sin_angle);
    point.setZ(Zc + (tmp_x - Xc) * sin_angle + (point.z() - Zc) * cos_angle);
}

void Transform::rotateByZ(QVector3D &point, const double angle, QVector3D &centerRotate)
{
    double Xc = centerRotate.x();
    double Yc = centerRotate.y();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(Xc + (point.x() - Xc) * cos_angle + (point.y() - Yc) * sin_angle);
    point.setY(Yc + (Xc - tmp_x) * sin_angle + (point.y() - Yc) * cos_angle);
}

void Transform::rotate(QVector3D &point, const Rotate &rotate, QVector3D &centerRotate)
{
    rotateByX(point, rotate.xAngle, centerRotate);
    rotateByY(point, rotate.yAngle, centerRotate);
    rotateByZ(point, rotate.zAngle, centerRotate);
}

void Transform::move(QVector3D &point, const Move &move)
{
    point.setX(point.x() + move.dx);
    point.setY(point.y() + move.dy);
    point.setZ(point.z() + move.dz);
}

void Transform::scale(QVector3D &point, const Scale &scale, QVector3D &centerScale)
{
    point.setX(scale.kx * point.x() + (1 - scale.kx) * centerScale.x());
    point.setY(scale.ky * point.y() + (1 - scale.ky) * centerScale.y());
    point.setZ(scale.kz * point.z() + (1 - scale.kz) * centerScale.z());
}
