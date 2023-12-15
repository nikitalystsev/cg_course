#ifndef TRANSFORM_H
#define TRANSFORM_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <QVector3D>

struct rotate_t
{
    double xAngle;
    double yAngle;
    double zAngle;
};

struct move_t
{
    double dx;
    double dy;
    double dz;
};

struct scale_t
{
    double kx;
    double ky;
    double kz;
};

// класс трансформаций для точки
class Transform
{
public:
    static double toRadians(const double angle);

    // поворот
    static void rotateByX(QVector3D &point, const double angle);
    static void rotateByY(QVector3D &point, const double angle);
    static void rotateByZ(QVector3D &point, const double angle);
    static void rotate(QVector3D &point, const rotate_t &rotate);

    // перенос
    static void move(QVector3D &point, const move_t &move);

    // масштабирование в процессеы
};

#endif
