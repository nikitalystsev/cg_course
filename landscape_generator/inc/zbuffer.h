#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "../myMatrixLib/Matrix.hpp"
#include "plane.h"
#include "point.h"
#include <QColor>

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

    static int planeToBitmap(Plane &plane);
};

#endif
