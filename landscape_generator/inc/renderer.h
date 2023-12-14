#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "landscape.h"
#include "landscapeManager.h"
#include "plane.h"
#include "transform.h"
#include <QColor>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

struct Pixel
{
    QVector3D vec;
    double I;
};

// класс-отрисовщик
class Renderer
{
private:
    int _screenWidth, _screenHeight;
    Matrix<double> _zbuffer;
    QImage _framebuffer; // буфер кадра

    vector<Pixel> _getLineByBresenham(const QVector3D &p1, const QVector3D &p2);
    void _calcIntensityForLine(vector<Pixel> &line, const double &IPStart, const double &IPEnd);
    void _calcHeightForLine(vector<Pixel> &line, const double &ZPStart, const double &ZPEnd);
    void _renderPlane(const Plane &screenPlane, const vector<double> &heights, const vector<double> &intensity, const double waterlevel, const double maxHeight);
    int _getCorrectChannel(int _R, double I);

public:
    Renderer();
    Renderer(const int &width, const int &height);
    ~Renderer();

    int toSceneX(double originX);
    int toSceneY(double originY);

    void renderLandscape(Landscape &landscape, QGraphicsScene *scene);

    int getScreenWidth() const;
    int getScreenHeight() const;

    void clean();
};

#endif // __RENDERER_H__
