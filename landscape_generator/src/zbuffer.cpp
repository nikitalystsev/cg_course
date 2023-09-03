#include "../inc/zbuffer.h"
#include <climits>

Zbuffer::Zbuffer() :
    Zbuffer(default_width, default_height) {}

Zbuffer::Zbuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, height)
{
    const int rows = this->_zbuffer.get_rows();
    const int cols = this->_zbuffer.get_cols();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->_zbuffer[i][j] = INT_MIN;
            this->_framebuffer[i][j] = QColor(255, 255, 255);
        }
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

// static void swap_two_points(Point<int> &p1, Point<int> &p2)
//{
//     Point<int> tmp(p1);
//     p1 = p2;
//     p2 = tmp;
// }

// static void sort_by_non_decrease_y(Point<int> &p1, Point<int> &p2, Point<int> &p3)
//{
//     // Упорядочиваем точки p1(x1, y1),
//     // p2(x2, y2), p3(x3, y3)
//     if (p2.y() < p1.y())
//         swap_two_points(p1, p2);
//     // точки p1, p2 упорядочены
//     if (p3.y() < p1.y())
//         swap_two_points(p1, p3);
//     // точки p1, p3 упорядочены
//     // теперь p1 самая верхняя
//     // осталось упорядочить p2 и p3
//     if (p2.y() > p3.y())
//         swap_two_points(p2, p3);
// }

// int Zbuffer::convert_to_bitmap(const Point<double> &p1,
//                                const Point<double> &p2,
//                                const Point<double> &p3,
//                                plane_coeff_t coeff)
//{
//     Point<int> _p1, _p2, _p3;
//     _p1.set(p1.x(), p1.y(), p1.z());
//     _p2.set(p2.x(), p2.y(), p3.z());
//     _p3.set(p3.x(), p3.y(), p3.z());

//    // сортируем по неубыванию ординаты
//    sort_by_non_decrease_y(_p1, _p2, _p3);

//    return 0;
//}
