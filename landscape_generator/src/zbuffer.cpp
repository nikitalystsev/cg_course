#include "../inc/zbuffer.h"

ZBuffer::ZBuffer() :
    ZBuffer(default_width, default_height) {}

ZBuffer::ZBuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, vector<double>(height, INT_MIN)),
    _framebuffer(width, vector<QColor>(height, QColor(255, 255, 255)))
{
}

ZBuffer::~ZBuffer() {}

void ZBuffer::calcZBufferByPlane(Plane &plane)
{
    //    std::cout << "call calcDepth" << std::endl;

    // получаем координаты описывающего прямоугольника
    Point2D<double> pMin = plane.getPMin();
    Point2D<double> pMax = plane.getPMax();

    double p1X = plane.getP1().getX(), p1Y = plane.getP1().getY();
    double p2X = plane.getP2().getX(), p2Y = plane.getP2().getY();
    double p3X = plane.getP3().getX(), p3Y = plane.getP3().getY();

    // обходим только ту часть матрицы z-буфера, что является
    // описывающим прямоугольником
    for (int x = pMin.getX(); x <= pMax.getX(); ++x)
        for (int y = pMin.getY(); y <= pMax.getY(); ++y)
        {
            // уравнения сторон
            double aSide = x * (p1Y - p2Y) + y * (p2X - p1X) + (p1X * p2Y - p2X * p1Y);
            double bSide = x * (p2Y - p3Y) + y * (p3X - p2X) + (p2X * p3Y - p3X * p2Y);
            double cSide = x * (p3Y - p1Y) + y * (p1X - p3X) + (p3X * p1Y - p1X * p3Y);

            // принадлежность точки треугольнику
            int isInTriangle = (aSide >= 0 && bSide >= 0 && cSide >= 0) || (aSide < 0 && bSide < 0 && cSide < 0);

            if (isInTriangle)
            {
                double z = plane.caclZ(x, y);

                if (z > this->_zbuffer[x][y])
                {
                    this->_zbuffer[x][y] = z;
                    this->_framebuffer[x][y] = QColor(143, 16, 16);
                }
            }
        }
}

void ZBuffer::clean()
{
    for (int i = 0; i < this->_width; ++i)
        for (int j = 0; j < this->_height; ++j)
        {
            this->_zbuffer[i][j] = INT_MIN;
            this->_framebuffer[i][j] = QColor(255, 255, 255);
        }
}

int ZBuffer::getWidth() const
{
    return this->_width;
}

int ZBuffer::getHeight() const
{
    return this->_height;
}

QColor ZBuffer::getFramebufElem(const int x, const int y) const
{
    return this->_framebuffer[x][y];
}
