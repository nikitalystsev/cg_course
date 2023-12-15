#ifndef PLANE_H
#define PLANE_H

#include <QVector2D>
#include <QVector3D>

#include <iostream>

class Plane
{
private:
    QVector3D _p1, _p2, _p3; // точки плоскости на экране
    double _A, _B, _C, _D;   // коэф-ты плоскости (они же координаты внешней нормали)

    // точки описывающего прямоугольника
    QVector2D _pMin, _pMax;

    void _calcPlaneCoeff();
    // метод, определяющий координаты описывающего треугольную плоскость прямоугольника
    void _defCircumRect();

public:
    Plane(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3);

    double caclZ(const double x, const double y) const;

    QVector3D getP1() const { return this->_p1; };
    QVector3D getP2() const { return this->_p2; };
    QVector3D getP3() const { return this->_p3; };

    QVector2D getPMin() const { return this->_pMin; };
    QVector2D getPMax() const { return this->_pMax; };

    double getA() const { return this->_A; };
    double getB() const { return this->_B; };
    double getC() const { return this->_C; };
    double getD() const { return this->_D; };
};

#endif
