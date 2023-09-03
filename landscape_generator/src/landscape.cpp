#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(default_width, default_lenght)
{
}

Landscape::Landscape(const int width, const int lenght) :
    _width(width), _lenght(lenght), _map(width + 1, lenght + 1)
{
}

Landscape::~Landscape() {}

void Landscape::generateHeightMap()
{
    const int rows = this->_map.get_rows();
    const int cols = this->_map.get_cols();

    PerlinNoise alg(22, 5, 1, 4, 1, 0.25);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double nx = i / (double)this->_width - 0.5;
            double ny = j / (double)this->_lenght - 0.5;

            double height = alg.generateHeight(nx, ny);

            //            std::cout << "height = " << height << std::endl;
            height *= 1000;

            this->_map[i][j].setPoint3D(i * this->poly_size, j * this->poly_size, height);
        }
}

void Landscape::draw(QGraphicsScene *scene)
{
    //    std::cout << "call draw" << std::endl;

    Matrix<Point3D<double>> screenMap = this->_mapToScreen();
    this->_drawMap(screenMap, scene);
}

Matrix<Point3D<double>> Landscape::_mapToScreen() const
{
    Matrix<Point3D<double>> tmp(this->_map);

    for (int i = 0; i < tmp.get_rows(); ++i)
        for (int j = 0; j < tmp.get_cols(); ++j)
            pointToIsometric(tmp[i][j]);

    return tmp;
}

void Landscape::_drawLine(const Point3D<double> &p1,
                          const Point3D<double> &p2,
                          QGraphicsScene *scene) const
{
    scene->addLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

void Landscape::_drawMap(const Matrix<Point3D<double>> &screenMap, QGraphicsScene *scene) const
{
    //    std::cout << "call _drawMap" << std::endl;
    scene->clear();

    const int rows = this->_map.get_rows();
    const int cols = this->_map.get_cols();

    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            this->_drawLine(screenMap[i][j], screenMap[i + 1][j], scene);
            this->_drawLine(screenMap[i + 1][j], screenMap[i + 1][j + 1], scene);
            this->_drawLine(screenMap[i + 1][j + 1], screenMap[i][j + 1], scene);
            this->_drawLine(screenMap[i][j + 1], screenMap[i][j], scene);
            this->_drawLine(screenMap[i][j], screenMap[i + 1][j + 1], scene);
        }
}
