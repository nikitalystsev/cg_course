#include "../inc/renderer.h"

Renderer::Renderer() :
    _screenWidth(1311), _screenHeight(781),
    _zbuffer(1311, vector<double>(781, INT_MIN)), _framebuffer(1311, 781, QImage::Format_RGB32)
{
}

Renderer::Renderer(const int &width, const int &height) :
    _screenWidth(width), _screenHeight(height), _zbuffer(width, vector<double>(height, INT_MIN)), _framebuffer(width, height, QImage::Format_RGB32)
{
}

Renderer::~Renderer() {}

void Renderer::_renderPlane(const Plane &screenPlane, const vector<double> &heights, const vector<double> &intensity, const double waterlevel, const double maxHeight)
{
    // получаем координаты описывающего прямоугольника
    QVector2D pMin = screenPlane.getPMin();
    QVector2D pMax = screenPlane.getPMax();

    QVector3D p1 = screenPlane.getP1();
    QVector3D p2 = screenPlane.getP2();
    QVector3D p3 = screenPlane.getP3();

    double p1X = p1.x(), p1Y = p1.y();
    double p2X = p2.x(), p2Y = p2.y();
    double p3X = p3.x(), p3Y = p3.y();

    double deltaY12 = p1Y - p2Y, deltaX21 = p2X - p1X, var1 = p1X * p2Y - p2X * p1Y;
    double deltaY23 = p2Y - p3Y, deltaX32 = p3X - p2X, var2 = p2X * p3Y - p3X * p2Y;
    double deltaY31 = p3Y - p1Y, deltaX13 = p1X - p3X, var3 = p3X * p1Y - p1X * p3Y;

    // растеризуем линии по алгоритму Брезенхема, взятому с Вики
    vector<Pixel> line1 = this->_getLineByBresenham(p1, p2);
    vector<Pixel> line2 = this->_getLineByBresenham(p2, p3);
    vector<Pixel> line3 = this->_getLineByBresenham(p3, p1);

    // расчет интенсивности на ребрах
    this->_calcIntensityForLine(line1, intensity[0], intensity[1]);
    this->_calcIntensityForLine(line2, intensity[1], intensity[2]);
    this->_calcIntensityForLine(line3, intensity[2], intensity[0]);

    // расчет высот на ребрах
    this->_calcHeightForLine(line1, heights[0], heights[1]);
    this->_calcHeightForLine(line2, heights[1], heights[2]);
    this->_calcHeightForLine(line3, heights[2], heights[0]);

    // Объединение трех векторов
    vector<Pixel> allLines;
    allLines.insert(allLines.end(), line1.begin(), line1.end());
    allLines.insert(allLines.end(), line2.begin(), line2.end());
    allLines.insert(allLines.end(), line3.begin(), line3.end());

#pragma omp parallel for

    // обходим только ту часть матрицы z-буфера, что является
    // описывающим прямоугольником
    for (int y = pMin.y(); y <= pMax.y(); ++y)
    {
        // вектор точек ребер на сканирующей строке
        // Выборка точек с заданным y с использованием std::copy_if
        vector<Pixel> yn;
        std::copy_if(allLines.begin(), allLines.end(), std::back_inserter(yn), [y](const Pixel &point)
                     { return point.vec.y() == y; });

        // Сортировка вектора объектов Point по x
        std::sort(yn.begin(), yn.end(), [](const Pixel &a, const Pixel &b)
                  { return a.vec.x() < b.vec.x(); });

        double deltaX = yn[yn.size() - 1].vec.x() - yn[0].vec.x();
        double invDeltaX = 1.0 / deltaX;
        double x0 = yn[0].vec.x(), I0 = yn[0].I;
        double Z0 = yn[0].vec.z();

        //#pragma omp parallel for
        for (int x = pMin.x(); x <= pMax.x(); ++x)
        {
            // уравнения сторон
            double aSide = x * deltaY12 + y * deltaX21 + var1;
            double bSide = x * deltaY23 + y * deltaX32 + var2;
            double cSide = x * deltaY31 + y * deltaX13 + var3;

            // принадлежность точки треугольнику
            int isInTriangle = (aSide >= 0 && bSide >= 0 && cSide >= 0) || (aSide < 0 && bSide < 0 && cSide < 0);

            if (isInTriangle)
            {
                double u = (x - x0) * invDeltaX;
                double I = (deltaX == 0) ? I0 : (u * I0 + (1 - u) * yn[yn.size() - 1].I);
                double Z = (deltaX == 0) ? Z0 : (u * Z0 + (1 - u) * yn[yn.size() - 1].vec.z());

                double z = screenPlane.caclZ(x, y);

                if (z > this->_zbuffer[x][y])
                {
                    this->_zbuffer[x][y] = z;

                    int r, g, b;

                    if (Z == waterlevel)
                    {
                        r = this->_getCorrectChannel(7, I);
                        g = this->_getCorrectChannel(11, I);
                        b = this->_getCorrectChannel(117, I);
                    }
                    else if (Z > waterlevel && Z < waterlevel + 5)
                    {
                        r = this->_getCorrectChannel(64, I);
                        g = this->_getCorrectChannel(56, I);
                        b = this->_getCorrectChannel(15, I);
                    }
                    else if (Z < maxHeight && Z > maxHeight - 100)
                    {
                        r = this->_getCorrectChannel(197, I);
                        g = this->_getCorrectChannel(229, I);
                        b = this->_getCorrectChannel(227, I);
                    }
                    else
                    {
                        r = this->_getCorrectChannel(12, I);
                        g = this->_getCorrectChannel(71, I);
                        b = this->_getCorrectChannel(14, I);
                    }

                    this->_framebuffer.setPixelColor(x, y, QColor(r, g, b));
                }
            }
        }
    }
}

int Renderer::_getCorrectChannel(int _R, double I)
{
    int _r = _R * I;

    if (_r > 255)
    {
        _r = 255;
    }
    else if (_r < 0)
    {
        _r = 0;
    }

    return _r;
}

vector<Pixel> Renderer::_getLineByBresenham(const QVector3D &p1, const QVector3D &p2)
{
    vector<Pixel> result;

    int x1 = p1.x(), x2 = p2.x();
    int y1 = p1.y(), y2 = p2.y();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;

    int error = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        Pixel pixel;
        pixel.vec = QVector3D(x1, y1, 0);
        pixel.I = 0;

        result.push_back(pixel);

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

    Pixel pixel;
    pixel.vec = QVector3D(x2, y2, 0);
    pixel.I = 0;

    result.push_back(pixel);

    return result;
}

void Renderer::_calcIntensityForLine(vector<Pixel> &line, const double &IPStart, const double &IPEnd)
{
    int lineSize = line.size();

    for (int i = 0; i < lineSize; ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (double)(i + 1) / lineSize;
        // устанавливаем значение интенсивности в текущей точке на ребре
        line[i].I = u * IPStart + (1 - u) * IPEnd;
    }
}

void Renderer::_calcHeightForLine(vector<Pixel> &line, const double &ZPStart, const double &ZPEnd)
{
    int lineSize = line.size();

    for (int i = 0; i < lineSize; ++i)
    {
        // буквально такая же интерполяция для высот
        double u = (double)(i + 1) / lineSize;
        // устанавливаем значение высоты в текущей точке на ребре
        line[i].vec.setZ(u * ZPStart + (1 - u) * ZPEnd);
    }
}

void Renderer::_calcCenterPoint(const Matrix<QVector3D> &screenMap)
{
    int xMin = screenMap[0][0].x(), yMin = screenMap[0][0].y();
    int xMax = xMin, yMax = yMin;

    for (const auto &row : screenMap)
        for (const auto &point : row)
        {
            int currX = point.x();
            int currY = point.y();

            xMin = std::min(xMin, currX);
            yMin = std::min(yMin, currY);
            xMax = std::max(xMax, currX);
            yMax = std::max(yMax, currY);
        }

    this->_centerPoint.setX((xMin + xMax) / 2);
    this->_centerPoint.setY((yMin + yMax) / 2);
}

void Renderer::_movePointToCenter(QVector3D &point)
{
    double x = point.x() + this->_screenWidth / 2 - this->_centerPoint.x();
    double y = point.y() + this->_screenHeight / 2 - this->_centerPoint.y();

    point.setX(x), point.setY(y);
}

Matrix<QVector3D> Renderer::mapToScreen(Matrix<QVector3D> &map)
{
    Matrix<QVector3D> tmp(map);

    int rows = map.size();
    int cols = map[0].size();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            Transform::pointToIsometric(tmp[i][j]);
        }

    this->_calcCenterPoint(tmp);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->_movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

void Renderer::renderLandscape(Landscape &landscape, QGraphicsScene *scene)
{
    this->clean();

    Matrix<QVector3D> &map = landscape.getHeightMap();
    Matrix<QVector3D> screenMap = this->mapToScreen(map);
    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();

    int waterlevel = landscape.getWaterlevel();
    int maxHeight = landscape.getMaxHeight();

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

            // определяем высоты вершин квадрата
            double z1 = map[i][j].z();
            double z2 = map[i + 1][j].z();
            double z3 = map[i + 1][j + 1].z();
            double z4 = map[i][j + 1].z();

            // определяем интенсивности вершин квадрата
            double I1 = intensityVertexMap[i][j];
            double I2 = intensityVertexMap[i + 1][j];
            double I3 = intensityVertexMap[i + 1][j + 1];
            double I4 = intensityVertexMap[i][j + 1];

            heights1 = {z1, z2, z3};
            heights2 = {z1, z4, z3};

            intensities1 = {I1, I2, I3};
            intensities2 = {I1, I4, I3};

            //  определяем текущее состояние z-буффера
            this->_renderPlane(plane1, heights1, intensities1, waterlevel, maxHeight);
            this->_renderPlane(plane2, heights2, intensities2, waterlevel, maxHeight);
        }

    QPixmap pixmap = QPixmap::fromImage(this->_framebuffer);
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
            this->_framebuffer.setPixelColor(i, j, QColor(0, 0, 0));
        }
}
