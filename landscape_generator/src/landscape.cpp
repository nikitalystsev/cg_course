#include "../inc/landscape.h"

Landscape::Landscape() :
    Landscape(100, 100, 0)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _rows(width + 1), _cols(lenght + 1),
    _heightMap(width + 1, vector<double>(lenght + 1)),
    _screenHeightMap(width + 1, vector<QVector3D>(lenght + 1)),
    _withoutWaterHeightMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<QVector3D, QVector3D>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<QVector3D>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1))
{
}

Landscape::~Landscape() {}

Landscape::Landscape(const Landscape &other)
{
    this->_heightMap = other._heightMap;
    this->_screenHeightMap = other._screenHeightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_maxHeight = other._maxHeight;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_operations = other._operations;
}

Landscape::Landscape(Landscape &&other) noexcept
{
    this->_heightMap = other._heightMap;
    this->_screenHeightMap = other._screenHeightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_maxHeight = other._maxHeight;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_operations = other._operations;
}

Landscape &Landscape::operator=(const Landscape &other)
{
    this->_heightMap = other._heightMap;
    this->_screenHeightMap = other._screenHeightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_maxHeight = other._maxHeight;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_operations = other._operations;

    return *this;
}

Landscape &Landscape::operator=(Landscape &&other) noexcept
{
    this->_heightMap = other._heightMap;
    this->_screenHeightMap = other._screenHeightMap;
    this->_withoutWaterHeightMap = other._withoutWaterHeightMap;
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_width = other._width;
    this->_lenght = other._lenght;
    this->_waterlevel = other._waterlevel;
    this->_maxHeight = other._maxHeight;
    this->_normalMap = other._normalMap;
    this->_normalVertexMap = other._normalVertexMap;
    this->_intensityVertexMap = other._intensityVertexMap;
    this->_operations = other._operations;

    return *this;
}

template <typename T>
void Landscape::resizeMatrix(Matrix<T> &matrix, int newWidth, int newLenght)
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
    this->_screenHeightMap.clear();
    this->_withoutWaterHeightMap.clear();
    this->_normalMap.clear();
    this->_normalVertexMap.clear();
    this->_intensityVertexMap.clear();

    this->resizeMatrix<double>(this->_heightMap, this->_rows, this->_cols);
    this->resizeMatrix<QVector3D>(this->_screenHeightMap, this->_rows, this->_cols);
    this->resizeMatrix<double>(this->_withoutWaterHeightMap, this->_rows, this->_cols);
    this->resizeMatrix<pair<QVector3D, QVector3D>>(this->_normalMap, this->_rows, this->_cols);
    this->resizeMatrix<QVector3D>(this->_normalVertexMap, this->_rows, this->_cols);
    this->resizeMatrix<double>(this->_intensityVertexMap, this->_rows, this->_cols);
}

void Landscape::setWaterlevel(const double waterlevel)
{
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

int Landscape::getCols() const
{
    return this->_cols;
}

int Landscape::getRows() const
{
    return this->_rows;
}

vector<Operation> &Landscape::getOperations()
{
    return this->_operations;
}

Matrix<double> &Landscape::getHeightMap()
{
    return this->_heightMap;
}

Matrix<QVector3D> &Landscape::getScreenHeightMap()
{
    return this->_screenHeightMap;
}

Matrix<double> &Landscape::getWithoutWaterHeightMap()
{
    return this->_withoutWaterHeightMap;
}

Matrix<pair<QVector3D, QVector3D>> &Landscape::getNormalMap()
{
    return this->_normalMap;
}

Matrix<QVector3D> &Landscape::getNormalVertexMap()
{
    return this->_normalVertexMap;
}

Matrix<double> &Landscape::getIntensityVertexMap()
{
    return this->_intensityVertexMap;
}
