#include "../inc/zbuffer.h"

Zbuffer::Zbuffer() :
    _width(default_width), _height(default_height), _zbuffer(default_width, default_height), _framebuffer(default_width, default_height)
{
    int rows = this->_zbuffer.get_rows();
    int cols = this->_zbuffer.get_cols();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double min_z = std::numeric_limits<int>().min();
            this->_zbuffer[i][j] = min_z;

            this->_framebuffer[i][j] = QColor(255, 255, 255);
        }
}

Zbuffer::Zbuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, height)
{
    int rows = this->_zbuffer.get_rows();
    int cols = this->_zbuffer.get_cols();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double min_z = std::numeric_limits<int>().min();
            this->_zbuffer[i][j] = min_z;

            this->_framebuffer[i][j] = QColor(255, 255, 255);
        }
}

Zbuffer::~Zbuffer() {}

// plane_coeff_t Zbuffer::calc_plane_coeff(const Point3D<double> &p1,
//                                         const Point3D<double> &p2,
//                                         const Point3D<double> &p3)
//{
//     // плоскость, заданная тремя точками p1, p2, p3
//     double m = p2.x() - p1.x(), n = p2.y() - p1.y(), p = p2.z() - p1.z();
//     double k = p3.x() - p1.x(), s = p3.y() - p1.y(), e = p3.z() - p1.z();

//    plane_coeff_t coeff;

//    coeff.A = n * e - s * p;
//    coeff.B = k * p - m * e;
//    coeff.C = m * s - k * n;
//    coeff.D = -(coeff.A * p1.x() + coeff.B * p1.y() + coeff.C * p1.z());

//    return coeff;
//}

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
