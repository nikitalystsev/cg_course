#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "landscape.h"
#include "landscapeManager.h"
#include "plane.h"
#include "transform.h"
#include <QColor>
#include <thread>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct ThreadParams
{
    int startRow;
    int endRow;
    int startCol;
    int endCol;
};

// класс-отрисовщик
class Renderer
{
private:
    int _screenWidth, _screenHeight;
    Matrix<double> _zbuffer;
    QImage _framebuffer; // буфер кадра

    Point2D<int> _centerPoint;

    void _calcCenterPoint(const Matrix<Point3D<double>> &screenMap);
    void _movePointToCenter(Point3D<double> &point);
    vector<Point3D<int>> _getLineByBresenham(const Point3D<double> &p1, const Point3D<double> &p2);
    void _calcIntensityForLine(vector<Point3D<int>> &line, const double &IPStart, const double &IPEnd);
    void _calcHeightForLine(vector<Point3D<int>> &line, const double &ZPStart, const double &ZPEnd);
    void _renderPlane(const Plane &screenPlane, const vector<double> &heights, const vector<double> &intensity, const double waterlevel, const double maxHeight);
    int _getCorrectChannel(int _R, double I);

public:
    Renderer();
    Renderer(const int &width, const int &height);
    ~Renderer();

    Matrix<Point3D<double>> mapToScreen(Matrix<Point3D<double>> &map);

    void renderLandscape(Landscape &landscape, QGraphicsScene *scene);
    int getScreenWidth() const;
    int getScreenHeight() const;

    void clean();
};

#endif // __RENDERER_H__