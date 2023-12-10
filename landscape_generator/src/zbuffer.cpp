#include "../inc/zbuffer.h"

ZBuffer::ZBuffer() :
    ZBuffer(default_width, default_height) {}

ZBuffer::ZBuffer(const int width, const int height) :
    _width(width), _height(height), _zbuffer(width, vector<double>(height, INT_MIN)),
    _framebuffer(width, vector<QColor>(height, QColor(255, 255, 255)))
{
}

ZBuffer::~ZBuffer() {}

void ZBuffer::calcZBufferByPlane(Plane &plane, double IP1, double IP2, double IP3)
{
    // получаем координаты описывающего прямоугольника
    Point2D<double> pMin = plane.getPMin();
    Point2D<double> pMax = plane.getPMax();

    Point3D<double> p1 = plane.getP1();
    Point3D<double> p2 = plane.getP2();
    Point3D<double> p3 = plane.getP3();

    double p1X = p1.getX(), p1Y = p1.getY();
    double p2X = p2.getX(), p2Y = p2.getY();
    double p3X = p3.getX(), p3Y = p3.getY();

    double deltaY12 = p1Y - p2Y, deltaX21 = p2X - p1X, var1 = p1X * p2Y - p2X * p1Y;
    double deltaY23 = p2Y - p3Y, deltaX32 = p3X - p2X, var2 = p2X * p3Y - p3X * p2Y;
    double deltaY31 = p3Y - p1Y, deltaX13 = p1X - p3X, var3 = p3X * p1Y - p1X * p3Y;

    // растеризуем линии по алгоритму Брезенхема, взятому с Вики
    vector<Point2D<int>> line1 = this->_getLineByBresenham(p1, p2);
    vector<Point2D<int>> line2 = this->_getLineByBresenham(p2, p3);
    vector<Point2D<int>> line3 = this->_getLineByBresenham(p3, p1);

    // расчет интенсивности на ребрах
    this->_calcIntensityForLine(line1, IP1, IP2);
    this->_calcIntensityForLine(line2, IP2, IP3);
    this->_calcIntensityForLine(line3, IP3, IP1);

    // обходим только ту часть матрицы z-буфера, что является
    // описывающим прямоугольником
    for (int y = pMin.getY(); y <= pMax.getY(); ++y)
    {
        // вектор точек ребер на сканирующей строке
        vector<Point2D<int>> yn;

        for (int i = 0; i < line1.size(); ++i)
            if (line1[i].getY() == y)
                yn.push_back(line1[i]);

        for (int i = 0; i < line2.size(); ++i)
            if (line2[i].getY() == y)
                yn.push_back(line2[i]);

        for (int i = 0; i < line3.size(); ++i)
            if (line3[i].getY() == y)
                yn.push_back(line3[i]);

        // Сортировка вектора объектов Point по x
        std::sort(yn.begin(), yn.end(), [](const Point2D<int> &a, const Point2D<int> &b)
                  { return a.getX() < b.getX(); });

        for (int x = pMin.getX(); x <= pMax.getX(); ++x)
        {
            // уравнения сторон
            double aSide = x * deltaY12 + y * deltaX21 + var1;
            double bSide = x * deltaY23 + y * deltaX32 + var2;
            double cSide = x * deltaY31 + y * deltaX13 + var3;

            // принадлежность точки треугольнику
            int isInTriangle = (aSide >= 0 && bSide >= 0 && cSide >= 0) || (aSide < 0 && bSide < 0 && cSide < 0);

            if (isInTriangle)
            {
                double z = plane.caclZ(x, y);

                if (z > this->_zbuffer[x][y])
                {
                    this->_zbuffer[x][y] = z;

                    double I = 0;

                    if (yn[yn.size() - 1].getX() - yn[0].getX() == 0) // такое возможно только в вершинах треугольника
                        I = yn[0].getI();
                    else
                    {
                        double u = (double)(x - yn[0].getX()) / (yn[yn.size() - 1].getX() - yn[0].getX());
                        I = u * yn[0].getI() + (1 - u) * yn[yn.size() - 1].getI();
                    }

                    int r = 12 * I, g = 71 * I, b = 14 * I;

                    this->_framebuffer[x][y] = QColor(r, g, b);
                }
            }
        }
    }
}

vector<Point2D<int>> ZBuffer::_getLineByBresenham(Point3D<double> &p1, Point3D<double> &p2)
{
    vector<Point2D<int>> result;

    int x1 = p1.getX(), x2 = p2.getX();
    int y1 = p1.getY(), y2 = p2.getY();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;

    int error = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        result.push_back(Point2D<int>(x1, y1));

        int error2 = error * 2;
        if (error2 > -dy)
        {
            error -= dy;
            x1 += sx;
        }
        if (error2 < dx)
        {
            error += dx;
            y1 += sy;
        }
    }

    result.push_back(Point2D<int>(x2, y2));

    return result;
}

void ZBuffer::_calcIntensityForLine(vector<Point2D<int>> &line, double IPStart, double IPEnd)
{
    for (int i = 0; i < line.size(); ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (double)(i + 1) / line.size();
        // интенсивность в текущей точке на ребре
        double I = u * IPStart + (1 - u) * IPEnd;
        // устанавливаем значение I
        line[i].setI(I);
    }
}

void ZBuffer::calсFramebufferByPlane(Plane &plane, double IP1, double IP2, double IP3)
{
    // получаем координаты описывающего прямоугольника
    Point2D<double> pMin = plane.getPMin();
    Point2D<double> pMax = plane.getPMax();

    Point3D<double> p1 = plane.getP1();
    Point3D<double> p2 = plane.getP2();
    Point3D<double> p3 = plane.getP3();

    double p1X = p1.getX(), p1Y = p1.getY();
    double p2X = p2.getX(), p2Y = p2.getY();
    double p3X = p3.getX(), p3Y = p3.getY();

    // растеризуем линии по алгоритму Брезенхема, взятому с Вики
    vector<Point2D<int>> line1 = this->_getLineByBresenham(p1, p2);
    vector<Point2D<int>> line2 = this->_getLineByBresenham(p2, p3);
    vector<Point2D<int>> line3 = this->_getLineByBresenham(p3, p1);

    // расчет интенсивности на ребрах
    this->_calcIntensityForLine(line1, IP1, IP2);
    this->_calcIntensityForLine(line2, IP2, IP3);
    this->_calcIntensityForLine(line3, IP3, IP1);

    // обходим только ту часть матрицы буфера, что является
    // описывающим прямоугольником
    for (int y = pMin.getY(); y <= pMax.getY(); ++y)
    {
        // вектор точек ребер на сканирующей строке
        vector<Point2D<int>> yn;

        for (int i = 0; i < line1.size(); ++i)
            if (line1[i].getY() == y)
                yn.push_back(line1[i]);

        for (int i = 0; i < line2.size(); ++i)
            if (line2[i].getY() == y)
                yn.push_back(line2[i]);

        for (int i = 0; i < line3.size(); ++i)
            if (line3[i].getY() == y)
                yn.push_back(line3[i]);

        // Сортировка вектора объектов Point по x
        std::sort(yn.begin(), yn.end(), [](const Point2D<int> &a, const Point2D<int> &b)
                  { return a.getX() < b.getX(); });

        for (int x = pMin.getX(); x <= pMax.getX(); ++x)
        {
            // уравнения сторон
            double aSide = x * (p1Y - p2Y) + y * (p2X - p1X) + (p1X * p2Y - p2X * p1Y);
            double bSide = x * (p2Y - p3Y) + y * (p3X - p2X) + (p2X * p3Y - p3X * p2Y);
            double cSide = x * (p3Y - p1Y) + y * (p1X - p3X) + (p3X * p1Y - p1X * p3Y);

            // принадлежность точки треугольнику
            int isInTriangle = (aSide >= 0 && bSide >= 0 && cSide >= 0) || (aSide < 0 && bSide < 0 && cSide < 0);

            if (isInTriangle)
            {
                if (yn[yn.size() - 1].getX() - yn[0].getX() == 0) // такое возможно только в вершинах треугольника
                {
                    double I = yn[0].getI();
                    int r = 12 * I, g = 71 * I, b = 14 * I;

                    this->_framebuffer[x][y] = QColor(r, g, b);
                }
                else
                {
                    double u = (double)(x - yn[0].getX()) / (yn[yn.size() - 1].getX() - yn[0].getX());
                    double I = u * yn[0].getI() + (1 - u) * yn[yn.size() - 1].getI();

                    int r = 12 * I, g = 71 * I, b = 14 * I;

                    this->_framebuffer[x][y] = QColor(r, g, b);
                }
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
