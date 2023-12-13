#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "landscape.h"
#include "plane.h"
#include <QColor>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

// класс-отрисовщик
class Renderer
{
private:
    int _screenWidth, _screenHeight;
    Matrix<double> _zbuffer;
    Matrix<QColor> _framebuffer; // буфер кадра

public:
    Renderer(const int &width, const int &height);
    ~Renderer();

    void renderPlane(Plane &screenPlane, vector<double> &heights, vector<double> &intensity);
    vector<Point2D<int>> getLineByBresenham(Point3D<double> &p1, Point3D<double> &p2);
    void calcIntensityForLine(vector<Point2D<int>> &line, double IPStart, double IPEnd);

    void renderLandscape(Landscape &landscape);
    int getScreenWidth() const;
    int getScreenHeight() const;

    void clean();
};

#endif // __RENDERER_H__
