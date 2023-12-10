#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "plane.h"
#include "point/point2D.h"
#include "point/point3D.h"
#include "vector/vector3D.h"
#include <QColor>
#include <climits>
#include <utility>
#include <vector>

#define EPS 1e-7

template <typename T>
using vector = std::vector<T>;

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

// класс алгоритма z-buffer удаления невидимых линий
class ZBuffer
{
private:
    Matrix<double> _zbuffer;
    Matrix<QColor> _framebuffer; // буфер кадра
    int _width, _height;         // ширина и высота экрана вывода изображения

    vector<Point2D<int>> _getLineByBresenham(Point3D<double> &p1, Point3D<double> &p2);
    void _calcIntensityForLine(vector<Point2D<int>> &line, double IPStart, double IPEnd);

public:
    ZBuffer();
    ZBuffer(const int width, const int height);
    ~ZBuffer();

    void calcZBufferByPlane(Plane &plane, double IP1, double IP2, double IP3);
    void calсFramebufferByPlane(Plane &plane, double IP1, double IP2, double IP3);
    void clean();

    int getWidth() const;
    int getHeight() const;

    QColor getFramebufElem(const int x, const int y) const;

public:
    static const int default_width = 1311, default_height = 781;
};

#endif
