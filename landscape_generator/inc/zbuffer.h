#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "../my_matrix_lib/Matrix.hpp"
#include "point.h"
#include <QColor>

typedef struct plane_coeff_t plane_coeff_t;
struct plane_coeff_t // они же координаты вектора нормали
{
    double A;
    double B;
    double C;
    double D;
};

class Zbuffer
{
public:
    Zbuffer();
    Zbuffer(const int width, const int height);
    ~Zbuffer();

private:
    Matrix<double> _zbuffer;
    Matrix<QColor> _framebuffer; // буфер кадра
    int _width, _height;         // ширина и высота экрана

public:
    static const int default_width = 1031, default_height = 671;

    static plane_coeff_t calc_plane_coeff(const Point3D<double> &p1,
                                          const Point3D<double> &p2,
                                          const Point3D<double> &p3);
    static int convert_to_bitmap(const Point3D<double> &p1,
                                 const Point3D<double> &p2,
                                 const Point3D<double> &p3,
                                 plane_coeff_t coeff);
};

#endif
