#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "landscape.h"
#include "plane.h"
#include "transform.h"
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

    Point2D<int> _centerPoint;

public:
    Renderer();
    Renderer(const int &width, const int &height);
    ~Renderer();

    void renderPlane(Plane &screenPlane, vector<double> &heights, vector<double> &intensity);
    vector<Point2D<int>> getLineByBresenham(Point3D<double> &p1, Point3D<double> &p2);
    void calcIntensityForLine(vector<Point2D<int>> &line, double IPStart, double IPEnd);

    void calcCenterPoint(Matrix<Point3D<double>> &screenMap);
    void movePointToCenter(Point3D<double> &point);
    Matrix<Point3D<double>> mapToScreen(Matrix<Point3D<double>> &map);

    void renderLandscape(Landscape &landscape, QGraphicsScene *scene);
    int getScreenWidth() const;
    int getScreenHeight() const;

    void clean();
};

#endif // __RENDERER_H__
