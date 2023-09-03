#include "../inc/zbuffer.h"
#include <climits>

Zbuffer::Zbuffer() :
    Zbuffer(default_width, default_height) {}

Zbuffer::Zbuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, height, INT_MIN),
    _framebuffer(width, height, QColor(255, 255, 255))
{
}

Zbuffer::~Zbuffer() {}

planeCoeff_t Zbuffer::calcPlaneCoeff(const Point3D<double> &p1,
                                     const Point3D<double> &p2,
                                     const Point3D<double> &p3)
{
    // плоскость, заданная тремя точками p1, p2, p3
    double m = p2.getX() - p1.getX(), n = p2.getY() - p1.getY(), p = p2.getZ() - p1.getZ();
    double k = p3.getX() - p1.getX(), s = p3.getY() - p1.getY(), e = p3.getZ() - p1.getZ();

    planeCoeff_t coeff;

    coeff.A = n * e - s * p;
    coeff.B = k * p - m * e;
    coeff.C = m * s - k * n;
    coeff.D = -(coeff.A * p1.getX() + coeff.B * p1.getY() + coeff.C * p1.getZ());

    return coeff;
}

int Zbuffer::planeToBitmap(const Point3D<double> &p1,
                           const Point3D<double> &p2,
                           const Point3D<double> &p3,
                           planeCoeff_t coeff)
{
    Point3D<int> lowPoint(p1), midPoint(p2), highPoint(p3);

    // сортируем по неубыванию ординаты
    //    sortByIncreaseY(lowPoint, midPoint, highPoint);

    return 0;
}
