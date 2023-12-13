#include "../inc/renderer.h"

Renderer::Renderer() :
    _screenWidth(1311), _screenHeight(781),
    _zbuffer(1311, vector<double>(781, INT_MIN)), _framebuffer(1311, vector<QColor>(781, QColor(255, 255, 255)))
{
}

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

    int rows = screenMap.size();
    int cols = screenMap[0].size();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
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

void Renderer::movePointToCenter(Point3D<double> &point)
{
    double x = point.getX() + this->_screenWidth / 2 - this->_centerPoint.getX();
    double y = point.getY() + this->_screenHeight / 2 - this->_centerPoint.getY();
    double z = point.getZ();

    point.set(x, y, z);
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

    this->calcCenterPoint(tmp);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

void Renderer::renderLandscape(Landscape &landscape, QGraphicsScene *scene)
{
    this->clean();

    Matrix<Point3D<double>> screenMap = this->mapToScreen(landscape.getMap());
    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();

    int width = landscape.getWidth();
    int height = landscape.getLenght();

    vector<double> heights1, heights2;
    vector<double> intensities1, intensities2;

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenMap[i][j], screenMap[i + 1][j], screenMap[i + 1][j + 1]);
            Plane plane2(screenMap[i][j], screenMap[i][j + 1], screenMap[i + 1][j + 1]);

            heights1 = {screenMap[i][j].getZ(),
                        screenMap[i + 1][j].getZ(),
                        screenMap[i + 1][j + 1].getZ()};
            heights2 = {screenMap[i][j].getZ(),
                        screenMap[i][j + 1].getZ(),
                        screenMap[i + 1][j + 1].getZ()};

            // определяем интенсивности вершин квадрата
            double I1 = intensityVertexMap[i][j];
            double I2 = intensityVertexMap[i + 1][j];
            double I3 = intensityVertexMap[i + 1][j + 1];
            double I4 = intensityVertexMap[i][j + 1];

            intensities1 = {I1, I2, I3};
            intensities2 = {I1, I4, I3};

            //  определяем текущее состояние z-буффера
            this->renderPlane(plane1, heights1, intensities1);
            this->renderPlane(plane2, heights2, intensities2);
        }

    QPixmap pixmap(this->_screenWidth, this->_screenHeight);
    pixmap.fill(Qt::white);

    QImage image(this->_screenWidth, this->_screenHeight, QImage::Format_RGB32);
    int r, g, b;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            this->_framebuffer[i][j].getRgb(&r, &g, &b);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
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
