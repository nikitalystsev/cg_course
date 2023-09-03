#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "../my_matrix_lib/Matrix.hpp"
#include "point.h"
#include <QColor>

typedef struct planeCoeff_t planeCoeff_t;
struct planeCoeff_t // они же координаты вектора нормали (без D)
{
    double A;
    double B;
    double C;
    double D;
};

class Zbuffer
{
private:
    Matrix<double> _zbuffer;
    Matrix<QColor> _framebuffer; // буфер кадра
    int _width, _height;         // ширина и высота экрана

public:
    Zbuffer();
    Zbuffer(const int width, const int height);
    ~Zbuffer();

public:
    static const int default_width = 1031, default_height = 671;

    static planeCoeff_t calcPlaneCoeff(const Point3D<double> &p1,
                                       const Point3D<double> &p2,
                                       const Point3D<double> &p3);
    //    static int convert_to_bitmap(const Point3D<double> &p1,
    //                                 const Point3D<double> &p2,
    //                                 const Point3D<double> &p3,
    //                                 planeCoeff_t coeff);
};

#endif
