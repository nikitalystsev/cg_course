#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(100, 100, 0)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _rows(width + 1), _cols(lenght + 1),
    _map(width + 1, vector<Point3D<double>>(lenght + 1)),
    _withoutWaterlevelMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1))
{
}

Landscape::~Landscape() {}

Landscape::Landscape(const Landscape &other)
{
    this->_map = other._map;
    this->_withoutWaterlevelMap = other._withoutWaterlevelMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_maxHeight = other._maxHeight;
}

Landscape::Landscape(Landscape &&other) noexcept
{
    this->_map = other._map;
    this->_withoutWaterlevelMap = other._withoutWaterlevelMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_maxHeight = other._maxHeight;
}

Landscape &Landscape::operator=(const Landscape &other)
{
    this->_map = other._map;
    this->_withoutWaterlevelMap = other._withoutWaterlevelMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_maxHeight = other._maxHeight;

    return *this;
}

Landscape &Landscape::operator=(Landscape &&other) noexcept
{
    this->_map = other._map;
    this->_withoutWaterlevelMap = other._withoutWaterlevelMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_maxHeight = other._maxHeight;

    return *this;
}

template <typename T>
void resizeMatrix(Matrix<T> &matrix, int newWidth, int newLenght)
{
    matrix.resize(newWidth);

    for (int i = 0; i < newWidth; ++i)
        matrix[i].resize(newLenght);
}

void Landscape::resize(const int width, const int lenght)
{
    this->_width = width;
    this->_lenght = lenght;
    this->_rows = width + 1;
    this->_cols = lenght + 1;

    this->_map.clear();
    this->_withoutWaterlevelMap.clear();
    this->_normalMap.clear();
    this->_normalVertexMap.clear();
    this->_intensityVertexMap.clear();

    resizeMatrix<Point3D<double>>(this->_map, this->_rows, this->_cols);
    resizeMatrix<double>(this->_withoutWaterlevelMap, this->_rows, this->_cols);
    resizeMatrix<pair<Vector3D<double>, Vector3D<double>>>(this->_normalMap, this->_rows, this->_cols);
    resizeMatrix<Vector3D<double>>(this->_normalVertexMap, this->_rows, this->_cols);
    resizeMatrix<double>(this->_intensityVertexMap, this->_rows, this->_cols);
}

// Matrix<Point3D<double>> Landscape::_mapToScreen()
//{
//     Matrix<Point3D<double>> tmp(this->_map);

//    Matrix<double> view = this->_camera.getView();
//    Matrix<double> projection = this->_camera.getProjection();

//    Matrix<double> vp = this->_matrixMul(view, projection);

//    for (int i = 0; i < this->_rows; ++i)
//        for (int j = 0; j < this->_cols; ++j)
//        {
//            Matrix<double> vertex(1, vector<double>(4, 0));
//            vertex[0][0] = tmp[i][j].getX();
//            vertex[0][1] = tmp[i][j].getY();
//            vertex[0][2] = tmp[i][j].getZ();

//            Matrix<double> res = this->_matrixMul(vertex, vp);

//            tmp[i][j].setX(res[0][0]);
//            tmp[i][j].setY(res[0][1]);
//            tmp[i][j].setZ(res[0][2]);

//            //            Transform::pointToIsometric(tmp[i][j]);
//        }

//    this->_calcCenterPoint(tmp);

//    for (int i = 0; i < this->_rows; ++i)
//        for (int j = 0; j < this->_cols; ++j)
//            this->_movePointToCenter(tmp[i][j]);

//    return tmp;
//}

// void Landscape::_shiftPointToOrigin(Point3D<double> &point)
//{
//     double x = point.getX() - this->_centerPoint.getX();
//     double y = point.getY() - this->_centerPoint.getY();
//     double z = point.getZ() - this->_centerPoint.getZ();

//    point.set(x, y, z);
//}

// void Landscape::_shiftPointBackToOrigin(Point3D<double> &point)
//{
//     double x = point.getX() + this->_centerPoint.getX();
//     double y = point.getY() + this->_centerPoint.getY();
//     double z = point.getZ() + this->_centerPoint.getZ();

//    point.set(x, y, z);
//}

int Landscape::getWaterlevel() const
{
    return this->_waterlevel;
}

void Landscape::setWaterlevel(const int waterlevel)
{
    this->_waterlevel = waterlevel;
}

void Landscape::updateWaterlevel(const int waterlevel)
{
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
            if (this->_withoutWaterlevelMap[i][j] < waterlevel)
                this->_map[i][j].set(i * 5, j * 5, waterlevel);
            else
                this->_map[i][j].set(i * 5, j * 5, this->_withoutWaterlevelMap[i][j]);

    this->_waterlevel = waterlevel;
}

int Landscape::getMaxHeight() const
{
    return this->_maxHeight;
}

void Landscape::setMaxHeight(const double maxHeight)
{
    this->_maxHeight = maxHeight;
}

int Landscape::getWidth() const
{
    return this->_width;
}

void Landscape::setWidth(const int width)
{
    this->_width = width;
}

int Landscape::getLenght() const
{
    return this->_lenght;
}

void Landscape::setLenght(const int lenght)
{
    this->_width = lenght;
}

void Landscape::setMap(const Matrix<Point3D<double>> &map)
{
    this->_map = map;
}

Matrix<Point3D<double>> &Landscape::getMap()
{
    return this->_map;
}

void Landscape::setNormalMap(const Matrix<pair<Vector3D<double>, Vector3D<double>>> &map)
{
    this->_normalMap = map;
}

Matrix<pair<Vector3D<double>, Vector3D<double>>> &Landscape::getNormalMap()
{
    return this->_normalMap;
}

void Landscape::setNormalVertexMap(const Matrix<Vector3D<double>> &map)
{
    this->_normalVertexMap = map;
}

Matrix<Vector3D<double>> &Landscape::getNormalVertexMap()
{
    return this->_normalVertexMap;
}

void Landscape::setIntensityVertexMap(const Matrix<double> &map)
{
    this->_intensityVertexMap = map;
}

Matrix<double> &Landscape::getIntensityVertexMap()
{
    return this->_intensityVertexMap;
}
