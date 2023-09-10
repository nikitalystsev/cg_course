#include "../inc/zbuffer.h"
#include "../inc/transform.h"
#include <climits>

Zbuffer::Zbuffer() :
    Zbuffer(default_width, default_height) {}

Zbuffer::Zbuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, height, INT_MIN),
    _framebuffer(width, height, QColor(255, 255, 255))
{
}

Zbuffer::~Zbuffer() {}

int Zbuffer::planeToBitmap(Plane &plane)
{
    Point3D<int> lowP(plane.getP1());
    Point3D<int> midP(plane.getP2());
    Point3D<int> highP(plane.getP3());

    //    сортируем по неубыванию ординаты
    sortByIncreaseY(lowP, midP, highP);

    return 0;
}
