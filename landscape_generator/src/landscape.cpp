#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(166, 166, 0)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _rows(width + 1), _cols(lenght + 1),
    _heightMap(width + 1, vector<Point3D<double>>(lenght + 1)),
    _withoutWaterHeightMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1))
{
}

Landscape::~Landscape() {}

Landscape::Landscape(const Landscape &other)
{
    this->_heightMap = other._heightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
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
    this->_heightMap = other._heightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
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
    this->_heightMap = other._heightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
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
    this->_heightMap = other._heightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
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

    this->_heightMap.clear();
    this->_withoutWaterHeightMap.clear();
    this->_normalMap.clear();
    this->_normalVertexMap.clear();
    this->_intensityVertexMap.clear();

    resizeMatrix<Point3D<double>>(this->_heightMap, this->_rows, this->_cols);
    resizeMatrix<double>(this->_withoutWaterHeightMap, this->_rows, this->_cols);
    resizeMatrix<pair<Vector3D<double>, Vector3D<double>>>(this->_normalMap, this->_rows, this->_cols);
    resizeMatrix<Vector3D<double>>(this->_normalVertexMap, this->_rows, this->_cols);
    resizeMatrix<double>(this->_intensityVertexMap, this->_rows, this->_cols);
}

void Landscape::updateWaterlevel(const double waterlevel)
{
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
            if (this->_withoutWaterHeightMap[i][j] < waterlevel)
                this->_heightMap[i][j].set(i * square, j * square, waterlevel);
            else
                this->_heightMap[i][j].set(i * square, j * square, this->_withoutWaterHeightMap[i][j]);

    this->_waterlevel = waterlevel;
}

double Landscape::getWaterlevel() const
{
    return this->_waterlevel;
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

Matrix<Point3D<double>> &Landscape::getHeightMap()
{
    return this->_heightMap;
}

Matrix<double> &Landscape::getWithoutWaterHeightMap()
{
    return this->_withoutWaterHeightMap;
}

Matrix<pair<Vector3D<double>, Vector3D<double>>> &Landscape::getNormalMap()
{
    return this->_normalMap;
}

Matrix<Vector3D<double>> &Landscape::getNormalVertexMap()
{
    return this->_normalVertexMap;
}

Matrix<double> &Landscape::getIntensityVertexMap()
{
    return this->_intensityVertexMap;
}
