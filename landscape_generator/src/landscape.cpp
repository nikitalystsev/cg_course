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
    _withoutWaterMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1))
{
}

Landscape::~Landscape() {}

Landscape::Landscape(const Landscape &other)
{
    this->_map = other._map;
    this->_withoutWaterMap = other._withoutWaterMap;
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
    this->_withoutWaterMap = other._withoutWaterMap;
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
    this->_withoutWaterMap = other._withoutWaterMap;
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
    this->_withoutWaterMap = other._withoutWaterMap;
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
    this->_withoutWaterMap.clear();
    this->_normalMap.clear();
    this->_normalVertexMap.clear();
    this->_intensityVertexMap.clear();

    resizeMatrix<Point3D<double>>(this->_map, this->_rows, this->_cols);
    resizeMatrix<double>(this->_withoutWaterMap, this->_rows, this->_cols);
    resizeMatrix<pair<Vector3D<double>, Vector3D<double>>>(this->_normalMap, this->_rows, this->_cols);
    resizeMatrix<Vector3D<double>>(this->_normalVertexMap, this->_rows, this->_cols);
    resizeMatrix<double>(this->_intensityVertexMap, this->_rows, this->_cols);
}

void Landscape::updateWaterlevel(const double waterlevel)
{
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
            if (this->_withoutWaterMap[i][j] < waterlevel)
                this->_map[i][j].set(i * square, j * square, waterlevel);
            else
                this->_map[i][j].set(i * square, j * square, this->_withoutWaterMap[i][j]);

    this->_waterlevel = waterlevel;
}

double Landscape::getWaterlevel() const
{
    return this->_waterlevel;
}

void Landscape::setWaterlevel(const double waterlevel)
{
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

Matrix<double> &Landscape::getWithoutWaterMap()
{
    return this->_withoutWaterMap;
}

void Landscape::setWithoutWaterMap(const Matrix<double> &withoutWaterMap)
{
    this->_withoutWaterMap = withoutWaterMap;
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
