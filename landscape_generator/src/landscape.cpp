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

vector<vector<Point3D<double>>> Landscape::_mapToScreen() const
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    vector<vector<Point3D<double>>> tmp(this->_map);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            int x = this->_map[i][j].getX();
            int y = this->_map[i][j].getY();
            int z = this->_map[i][j].getZ();

            tmp[i][j].set(x + 515 - 125, y + 330 - 125, z);
            Transform::pointToIsometric(tmp[i][j]);
        }

    return tmp;
}

void Landscape::_drawMap(QGraphicsScene *scene) const
{
    const int width = zBuf.getWidth();
    const int height = zBuf.getHeight();

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QImage image(width, height, QImage::Format_RGB32);
    int r, g, b;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            zBuf.getFramebufElem(i, j).getRgb(&r, &g, &b);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
}

void Landscape::_calcZBuffer(const vector<vector<Point3D<double>>> &screenMap)
{
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
            zBuf.calcZBufferByPlane(plane1);
            zBuf.calcZBufferByPlane(plane2);
        }
}

int Landscape::getWaterlevel() const
{
    return this->_waterlevel;
}

void Landscape::setWaterlevel(const int waterlevel)
{
    this->_waterlevel = waterlevel;
}

Point3D<double> Landscape::getCenterLandscapePoint() const
{
    return Point3D<double>(this->_centerLandscapePoint);
}

void Landscape::setCenterLandscapePoint(const Point3D<double> &centerLandscapePoint)
{
    this->_centerLandscapePoint = centerLandscapePoint;
}
