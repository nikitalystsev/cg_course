#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "plane.h"
#include "point/point2D.h"
#include "point/point3D.h"
#include <QColor>
#include <climits>
#include <vector>

template <typename T>
using vector = std::vector<T>;

// класс алгоритма z-buffer удаления невидимых линий
class ZBuffer
{
private:
    vector<vector<double>> _zbuffer;
    vector<vector<QColor>> _framebuffer; // буфер кадра
    int _width, _height;                 // ширина и высота экрана вывода изображения

public:
    ZBuffer();
    ZBuffer(const int width, const int height);
    ~ZBuffer();

    void calcZBufferByPlane(Plane &plane);
    void clean();

    int getWidth() const;
    int getHeight() const;

    QColor getFramebufElem(const int x, const int y) const;

public:
    static const int default_width = 1031, default_height = 671;
};

#endif
