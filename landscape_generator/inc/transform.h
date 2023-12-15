#ifndef TRANSFORM_H
#define TRANSFORM_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <QVector3D>

struct Rotate
{
    int xAngle;
    int yAngle;
    int zAngle;
};

struct Move
{
    int dx;
    int dy;
    int dz;
};

struct Scale
{
    double kx;
    double ky;
    double kz;
};

struct Operation
{
    int operIndex; // индекс выполняемой операции (0, 1, 2)
    int axisIndex; // индекс оси, на которой выполнялась операция (0, 1, 2)
    double value;  //  значение
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
    static void rotate(QVector3D &point, const Rotate &rotate);

    // перенос
    static void move(QVector3D &point, const Move &move);

    // масштабирование
    static void scale(QVector3D &point, const Scale &scale);
};

#endif
