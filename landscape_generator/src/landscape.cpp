#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(default_width, default_lenght, default_waterlevel)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _map(width + 1, vector<Point3D<double>>(lenght + 1))
{
}

Landscape::~Landscape() {}

void Landscape::generateHeightMap()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    PerlinNoise alg(22, 5, 1, 4, 1, 0.25);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double nx = i / (double)this->_width - 0.5;
            double ny = j / (double)this->_lenght - 0.5;

            double height = alg.generateNoise(nx, ny);

            height *= 1000;

            if (height < _waterlevel)
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, _waterlevel);
            else
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, height);
        }
}

void Landscape::draw(QGraphicsScene *scene)
{
    std::cout << "call draw" << std::endl;

    vector<vector<Point3D<double>>> screenMap = this->_mapToScreen();

    this->_calcZBuffer(screenMap);
    this->_drawMap(scene);
}

vector<vector<Point3D<double>>> Landscape::_mapToScreen()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    this->_calcCenterPoint();

    vector<vector<Point3D<double>>> tmp(this->_map);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->_shiftPointToOrigin(tmp[i][j]);

            Transform::pointToIsometric(tmp[i][j]);

            this->_shiftPointBackToOrigin(tmp[i][j]);
            this->_movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

void Landscape::_drawMap(QGraphicsScene *scene) const
{
    const int width = zBuffer.getWidth();
    const int height = zBuffer.getHeight();

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QImage image(width, height, QImage::Format_RGB32);
    int r, g, b;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            zBuffer.getFramebufElem(i, j).getRgb(&r, &g, &b);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
}

void Landscape::_calcZBuffer(const vector<vector<Point3D<double>>> &screenMap)
{
    zBuffer.clean();

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenMap[i][j], screenMap[i + 1][j], screenMap[i + 1][j + 1]);
            Plane plane2(screenMap[i + 1][j + 1], screenMap[i][j + 1], screenMap[i][j]);

            // определяем текущее состояние z-буффера
            zBuffer.calcZBufferByPlane(plane1);
            zBuffer.calcZBufferByPlane(plane2);
        }
}

void Landscape::_calcCenterPoint()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    Point3D<double> p1(this->_map[0][0]);
    Point3D<double> p2(this->_map[rows - 1][cols - 1]);

    this->_centerPoint.setX((p1.getX() + p2.getX()) / 2);
    this->_centerPoint.setY((p1.getY() + p2.getY()) / 2);

    double zMin = p1.getZ(), zMax = p1.getZ();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            if (this->_map[i][j].getZ() < zMin)
                zMin = this->_map[i][j].getZ();
            if (this->_map[i][j].getZ() > zMax)
                zMax = this->_map[i][j].getZ();
        }

    this->_centerPoint.setZ((zMin + zMax) / 2);
}

void Landscape::_shiftPointToOrigin(Point3D<double> &point)
{
    double x = point.getX() - this->_centerPoint.getX();
    double y = point.getY() - this->_centerPoint.getY();
    double z = point.getZ() - this->_centerPoint.getZ();

    point.set(x, y, z);
}

void Landscape::_shiftPointBackToOrigin(Point3D<double> &point)
{
    double x = point.getX() + this->_centerPoint.getX();
    double y = point.getY() + this->_centerPoint.getY();
    double z = point.getZ() + this->_centerPoint.getZ();

    point.set(x, y, z);
}

void Landscape::_movePointToCenter(Point3D<double> &point)
{
    double x = point.getX() + this->zBuffer.getWidth() / 2 - this->_centerPoint.getX();
    double y = point.getY() + this->zBuffer.getHeight() / 2 - this->_centerPoint.getY();
    double z = point.getZ();

    point.set(x, y, z);
}
int Landscape::getWaterlevel() const
{
    return this->_waterlevel;
}

void Landscape::setWaterlevel(const int waterlevel)
{
    this->_waterlevel = waterlevel;
}

Point3D<double> Landscape::getCenterPoint() const
{
    return Point3D<double>(this->_centerPoint);
}

void Landscape::setCenterPoint(const Point3D<double> &centerPoint)
{
    this->_centerPoint = centerPoint;
}
