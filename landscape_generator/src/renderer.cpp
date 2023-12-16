#include "../inc/renderer.h"

Renderer::Renderer() :
    Renderer(1089, 799)
{
}

Renderer::Renderer(const int &width, const int &height) :
    _screenWidth(width), _screenHeight(height), _zbuffer(width, vector<double>(height, INT_MIN)), _framebuffer(width, height, QImage::Format_RGB32)
{
    this->_xMax = this->_screenWidth / 2, this->_yMax = this->_screenHeight / 2;
    this->_xMin = -this->_xMax, this->_yMin = -this->_yMax;

    double kx = (double)(this->_screenWidth - 0) / (this->_xMax - this->_xMin);
    double ky = (double)(this->_screenHeight - 0) / (this->_yMax - this->_yMin);

    this->_km = std::min(kx, ky);
}

Renderer::~Renderer() {}

void Renderer::_renderPlane(const Plane &screenPlane, const vector<double> &heights, const vector<double> &intensity, const double waterlevel, const double maxHeight)
{
    //    std::cout << "[INFO] call _renderPlane" << std::endl;
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
        //        std::cout << "[INFO] in first cycle" << std::endl;
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

#pragma omp parallel for
        for (int x = pMin.x(); x <= pMax.x(); ++x)
        {
            // уравнения сторон
            double aSide = x * deltaY12 + y * deltaX21 + var1;
            double bSide = x * deltaY23 + y * deltaX32 + var2;
            double cSide = x * deltaY31 + y * deltaX13 + var3;

            int sceneX = this->toSceneX(x), sceneY = this->toSceneY(y);

            if (sceneX < 0 || sceneX >= this->_screenWidth || sceneY < 0 || sceneY >= this->_screenHeight)
                continue;

            // принадлежность точки треугольнику (если не принадлежит)
            if (!((aSide >= 0 && bSide >= 0 && cSide >= 0) || (aSide < 0 && bSide < 0 && cSide < 0)))
                continue;

            double u = (x - x0) * invDeltaX;
            double I = (deltaX == 0) ? I0 : (u * I0 + (1 - u) * yn[yn.size() - 1].I);
            double height = (deltaX == 0) ? Z0 : (u * Z0 + (1 - u) * yn[yn.size() - 1].vec.z());

            double z = screenPlane.caclZ(x, y);

            if (z > this->_zbuffer[sceneX][sceneY])
            {
                this->_zbuffer[sceneX][sceneY] = z;

                int r, g, b;

                if (height == waterlevel)
                {
                    r = this->_getCorrectChannel(7, I);
                    g = this->_getCorrectChannel(11, I);
                    b = this->_getCorrectChannel(117, I);
                }
                else if (height > waterlevel && height < waterlevel + 5)
                {
                    r = this->_getCorrectChannel(64, I);
                    g = this->_getCorrectChannel(56, I);
                    b = this->_getCorrectChannel(15, I);
                }
                else if (height < maxHeight && height > maxHeight - 100)
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

                this->_framebuffer.setPixelColor(sceneX, sceneY, QColor(r, g, b));
            }
        }
    }
}

int Renderer::_getCorrectChannel(int channel, double I)
{
    int value = channel * I;

    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;

    return value;
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

void Renderer::renderLandscape(Landscape &landscape, QGraphicsScene *scene)
{
    //    std::cout << "[INFO] call renderLandscape" << std::endl;

    this->clean();

    Matrix<double> &heightMap = landscape.getHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
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
            Plane plane1(screenHeightMap[i][j], screenHeightMap[i + 1][j], screenHeightMap[i + 1][j + 1]);
            Plane plane2(screenHeightMap[i][j], screenHeightMap[i][j + 1], screenHeightMap[i + 1][j + 1]);

            // определяем высоты вершин квадрата
            double z1 = heightMap[i][j];
            double z2 = heightMap[i + 1][j];
            double z3 = heightMap[i + 1][j + 1];
            double z4 = heightMap[i][j + 1];

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

void Renderer::clean()
{
    for (int i = 0; i < this->_screenWidth; ++i)
        for (int j = 0; j < this->_screenHeight; ++j)
        {
            this->_zbuffer[i][j] = INT_MIN;
            this->_framebuffer.setPixelColor(i, j, QColor(0, 0, 0));
        }
}

int Renderer::toSceneX(double originX)
{
    return 0 + (originX - this->_xMin) * this->_km;
}

int Renderer::toSceneY(double originY)
{
    return 0 + (originY - this->_yMin) * this->_km;
}
