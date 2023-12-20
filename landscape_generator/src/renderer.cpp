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
    double yMin = screenPlane.getPMin().y();
    double уMax = screenPlane.getPMax().y();
    double a = screenPlane.getA(), c = screenPlane.getC();

    // растеризуем линии по алгоритму Брезенхема, взятому с Вики
    vector<Pixel> line1 = this->_getLineByBresenham(screenPlane.getP1(), screenPlane.getP2());
    vector<Pixel> line2 = this->_getLineByBresenham(screenPlane.getP2(), screenPlane.getP3());
    vector<Pixel> line3 = this->_getLineByBresenham(screenPlane.getP3(), screenPlane.getP1());

    // расчет интенсивности на ребрах
    this->_calcParamsLine(line1, intensity[0], intensity[1], heights[0], heights[1]);
    this->_calcParamsLine(line2, intensity[1], intensity[2], heights[1], heights[2]);
    this->_calcParamsLine(line3, intensity[2], intensity[0], heights[2], heights[0]);

    // Объединение трех векторов
    vector<Pixel> allLines;
    allLines.insert(allLines.end(), line1.begin(), line1.end());
    allLines.insert(allLines.end(), line2.begin(), line2.end());
    allLines.insert(allLines.end(), line3.begin(), line3.end());

#pragma omp parallel for
    for (int y = yMin; y <= уMax; ++y)
    {
        vector<Pixel> yn;
        std::copy_if(allLines.begin(), allLines.end(), std::back_inserter(yn), [y](const Pixel &point)
                     { return point.vec.y() == y; });

        // Сортировка вектора объектов Point по x
        std::sort(yn.begin(), yn.end(), [](const Pixel &a, const Pixel &b)
                  { return a.vec.x() < b.vec.x(); });

        double deltaX = yn[yn.size() - 1].vec.x() - yn[0].vec.x();
        double invDeltaX = 1.0 / deltaX;
        double x0 = yn[0].vec.x(), I0 = yn[0].I, Z0 = yn[0].vec.z();

        double z = screenPlane.caclZ(x0, y);

        for (int x = x0; x <= yn[yn.size() - 1].vec.x(); ++x)
        {
            int sceneX = this->toSceneX(x), sceneY = this->toSceneY(y);

            if (sceneX < 0 || sceneX >= this->_screenWidth || sceneY < 0 || sceneY >= this->_screenHeight)
                continue;

            double u = (x - x0) * invDeltaX;
            double I = (deltaX == 0) ? I0 : (u * I0 + (1 - u) * yn[yn.size() - 1].I);
            double height = (deltaX == 0) ? Z0 : (u * Z0 + (1 - u) * yn[yn.size() - 1].vec.z());

            if (z > this->_zbuffer[sceneX][sceneY])
            {
                this->_zbuffer[sceneX][sceneY] = z;

                this->_framebuffer.setPixelColor(sceneX, sceneY, this->_getColorByHeight(I, height, maxHeight, waterlevel));
            }

            z -= (a / c);
        }
    }
}

QColor Renderer::_getColorByHeight(double I, double height, double maxHeight, double waterlevel)
{
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

    return QColor(r, g, b);
}

int Renderer::_getCorrectChannel(int channel, double I)
{
    int value = channel * I;

    value = (value > 255) ? 255 : value;
    value = (value < 0) ? 0 : value;

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
        result.push_back({QVector3D(x1, y1, 0), 0});

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

    result.push_back({QVector3D(x2, y2, 0), 0});

    return result;
}

void Renderer::_calcParamsLine(vector<Pixel> &line, const double &IPStart, const double &IPEnd, const double &ZPStart, const double &ZPEnd)
{
    int lineSize = line.size();

    for (int i = 0; i < lineSize; ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (double)(i + 1) / lineSize;
        // устанавливаем значение интенсивности в текущей точке на ребре
        line[i].I = u * IPStart + (1 - u) * IPEnd;
        line[i].vec.setZ(u * ZPStart + (1 - u) * ZPEnd);
    }
}

void Renderer::renderLandscape(Landscape &landscape, QGraphicsScene *scene)
{
    this->clean();

    Matrix<double> &heightMap = landscape.getHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();

    int waterlevel = landscape.getWaterlevel();
    int maxHeight = landscape.getMaxHeight();

    int width = landscape.getWidth(), lenght = landscape.getLenght();

    vector<double> heights1, heights2;
    vector<double> intensities1, intensities2;

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < lenght; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenHeightMap[i][j], screenHeightMap[i + 1][j], screenHeightMap[i + 1][j + 1]);
            Plane plane2(screenHeightMap[i][j], screenHeightMap[i][j + 1], screenHeightMap[i + 1][j + 1]);

            heights1 = {heightMap[i][j], heightMap[i + 1][j], heightMap[i + 1][j + 1]};
            heights2 = {heightMap[i][j], heightMap[i][j + 1], heightMap[i + 1][j + 1]};

            intensities1 = {intensityVertexMap[i][j], intensityVertexMap[i + 1][j], intensityVertexMap[i + 1][j + 1]};
            intensities2 = {intensityVertexMap[i][j], intensityVertexMap[i][j + 1], intensityVertexMap[i + 1][j + 1]};

            //  определяем текущее состояние z-буффера
            this->_renderPlane(plane1, heights1, intensities1, waterlevel, maxHeight);
            this->_renderPlane(plane2, heights2, intensities2, waterlevel, maxHeight);
        }

    scene->addPixmap(QPixmap::fromImage(this->_framebuffer));
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
