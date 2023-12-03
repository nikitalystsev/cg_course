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
    // получаем координаты описывающего прямоугольника
    Point2D<double> pMin = plane.getPMin();
    Point2D<double> pMax = plane.getPMax();

    double p1X = plane.getP1().getX(), p1Y = plane.getP1().getY();
    double p2X = plane.getP2().getX(), p2Y = plane.getP2().getY();
    double p3X = plane.getP3().getX(), p3Y = plane.getP3().getY();

    // обходим только ту часть матрицы z-буфера, что является
    // описывающим прямоугольником
    for (int y = pMin.getY(); y <= pMax.getY(); ++y)
    {
        int isCacl = 0; // флаг первого вычисленного значения на сканирующей строке
        double z;
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
                z = (isCacl) ? (this->_zbuffer[x][y - 1] - plane.getA() / plane.getC()) : plane.caclZ(x, y);

                if (z > this->_zbuffer[x][y])
                {
                    this->_zbuffer[x][y] = z;
                }

                isCacl = 1;
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

        //        std::cout << "[?] what?" << std::endl;

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

    //    std::cout << "[+] Success" << std::endl;

    return result;
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
    for (int i = 0; i < line1.size(); ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (i + 1) / line1.size();
        // интенсивность в текущей точке на ребре
        double I = u * IP1 + (1 - u) * IP2;
        // std::cout << "[=] line1[i]: I = " << I << std::endl;
        // устанавливаем значение I
        line1[i].setI(I);
    }

    for (int i = 0; i < line2.size(); ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (i + 1) / line2.size();
        // интенсивность в текущей точке на ребре
        double I = u * IP2 + (1 - u) * IP3;
        // std::cout << "[=] line2[i]: I = " << I << std::endl;
        // устанавливаем значение I
        line2[i].setI(I);
    }

    for (int i = 0; i < line3.size(); ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (i + 1) / line3.size();
        // интенсивность в текущей точке на ребре
        double I = u * IP3 + (1 - u) * IP1;
        // std::cout << "[=] line3[i]: I = " << I << std::endl;
        // устанавливаем значение I
        line3[i].setI(I);
    }

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

        // std::cout << "[=] yn size = " << yn.size() << std::endl;

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
                    this->_framebuffer[x][y] = QColor(12 * I, 71 * I, 14 * I);
                }
                else
                {
                    double u = (x - yn[0].getX()) / (yn[yn.size() - 1].getX() - yn[0].getX());
                    double I = u * yn[0].getI() + (1 - u) * yn[yn.size() - 1].getI();

                    this->_framebuffer[x][y] = QColor(12 * I, 71 * I, 14 * I);
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
