#include "../inc/renderer.h"

Renderer::Renderer(const int &width, const int &height) :
    _screenWidth(width), _screenHeight(height), _zbuffer(width, vector<double>(height, INT_MIN)), _framebuffer(width, vector<QColor>(height, QColor(255, 255, 255)))
{
}

Renderer::~Renderer() {}

void Renderer::renderPlane(Plane &screenPlane, vector<double> &heights, vector<double> &intensity)
{
    // получаем координаты описывающего прямоугольника
    Point2D<double> pMin = screenPlane.getPMin();
    Point2D<double> pMax = screenPlane.getPMax();

    Point3D<double> p1 = screenPlane.getP1();
    Point3D<double> p2 = screenPlane.getP2();
    Point3D<double> p3 = screenPlane.getP3();

    double p1X = p1.getX(), p1Y = p1.getY();
    double p2X = p2.getX(), p2Y = p2.getY();
    double p3X = p3.getX(), p3Y = p3.getY();

    double deltaY12 = p1Y - p2Y, deltaX21 = p2X - p1X, var1 = p1X * p2Y - p2X * p1Y;
    double deltaY23 = p2Y - p3Y, deltaX32 = p3X - p2X, var2 = p2X * p3Y - p3X * p2Y;
    double deltaY31 = p3Y - p1Y, deltaX13 = p1X - p3X, var3 = p3X * p1Y - p1X * p3Y;

    // растеризуем линии по алгоритму Брезенхема, взятому с Вики
    vector<Point2D<int>> line1 = this->getLineByBresenham(p1, p2);
    vector<Point2D<int>> line2 = this->getLineByBresenham(p2, p3);
    vector<Point2D<int>> line3 = this->getLineByBresenham(p3, p1);

    // расчет интенсивности на ребрах
    this->calcIntensityForLine(line1, intensity[0], intensity[1]);
    this->calcIntensityForLine(line2, intensity[1], intensity[2]);
    this->calcIntensityForLine(line3, intensity[2], intensity[0]);

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
                double z = screenPlane.caclZ(x, y);

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

vector<Point2D<int>> Renderer::getLineByBresenham(Point3D<double> &p1, Point3D<double> &p2)
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

void Renderer::calcIntensityForLine(vector<Point2D<int>> &line, double IPStart, double IPEnd)
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

void Renderer::calcCenterPoint(Matrix<Point3D<double>> &screenMap)
{
    int xMin = screenMap[0][0].getX(), yMin = screenMap[0][0].getY();
    int xMax = screenMap[0][0].getX(), yMax = screenMap[0][0].getY();

    int rows = map.size();
    int cols = map[0].size();

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            int currX = screenMap[i][j].getX(), currY = screenMap[i][j].getY();

            if (currX < xMin)
                xMin = currX;
            if (currY < yMin)
                yMin = currY;
            if (currX > xMax)
                xMax = currX;
            if (currY > yMax)
                yMax = currY;
        }

    this->_centerPoint.setX((xMin + xMax) / 2);
    this->_centerPoint.setY((yMin + yMax) / 2);
}

Matrix<Point3D<double>> Renderer::mapToScreen(Matrix<Point3D<double>> &map)
{
    Matrix<Point3D<double>> tmp(map);

    int rows = map.size();
    int cols = map[0].size();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            Transform::pointToIsometric(tmp[i][j]);
        }

    this->_calcCenterPoint(tmp);

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            this->_movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

void Renderer::renderLandscape(Landscape &landscape)
{
    this->clean();

    int width = landscape.getWidth();
    int height = landscape.getLenght();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenMap[i][j], screenMap[i + 1][j], screenMap[i + 1][j + 1]);
            Plane plane2(screenMap[i][j], screenMap[i][j + 1], screenMap[i + 1][j + 1]);

            // определяем интенсивности вершин квадрата
            double I1 = this->_intensityVertexMap[i][j];
            double I2 = this->_intensityVertexMap[i + 1][j];
            double I3 = this->_intensityVertexMap[i + 1][j + 1];
            double I4 = this->_intensityVertexMap[i][j + 1];

            //  определяем текущее состояние z-буффера
            _zBuffer.calcZBufferByPlane(plane1, I1, I2, I3);
            _zBuffer.calcZBufferByPlane(plane2, I1, I4, I3);
        }
}
int Renderer::getScreenWidth() const
{
    return this->_screenWidth;
}

int Renderer::getScreenHeight() const
{
    return this->_screenHeight;
}

void Renderer::clean()
{
    for (int i = 0; i < this->_screenWidth; ++i)
        for (int j = 0; j < this->_screenHeight; ++j)
        {
            this->_zbuffer[i][j] = INT_MIN;
            this->_framebuffer[i][j] = QColor(255, 255, 255);
        }
}
