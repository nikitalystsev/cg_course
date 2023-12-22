#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "transform.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QVector3D>
#include <cstdarg>
#include <utility>
#include <vector>

template <typename T>
using vector = std::vector<T>;

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

class Landscape
{
private:
    Matrix<double> _heightMap;                     // карта высот
    Matrix<double> _withoutWaterHeightMap = {};    // карта высот без учета уровня воды
    Matrix<QVector3D> _screenHeightMap;            // карта высот для вывода на экран
    Matrix<pair<QVector3D, QVector3D>> _normalMap; // матрица векторов внешней нормали к каждой из граней
    Matrix<QVector3D> _normalVertexMap;            // матрица векторов нормалей для каждой вершины
    Matrix<double> _intensityVertexMap;            // матрица интенсивности в каждой вершине
    int _rows, _cols;                              // размеры ландшафтных матриц
    int _width, _lenght;                           // длина и ширина карты высот (в количестве полигонов)
    double _waterlevel;                            // уровень моря
    double _maxHeight = 0;                         // максимальная сгенерированная высота
    int _maxGenHeight = 1000;                      // максимально возможная генерируемая высота
    vector<Operation> _operations;                 // цепочка преобразований, выполненных над ландшафтом
    int _square = 5;                               // размер одного квадрата

public:
    Landscape();
    Landscape(const int width, const int lenght, const int waterlevel);
    explicit Landscape(const Landscape &other);
    Landscape(Landscape &&other) noexcept;
    ~Landscape();

    Landscape &operator=(const Landscape &other);
    Landscape &operator=(Landscape &&other) noexcept;

    template <typename T>
    void resizeMatrix(Matrix<T> &matrix, int newWidth, int newLenght);
    void resize(const int width, const int lenght);

    double getWaterlevel() const { return this->_waterlevel; };
    void setWaterlevel(const double waterlevel) { this->_waterlevel = waterlevel; };

    int getMaxHeight() const { return this->_maxHeight; };
    void setMaxHeight(const double maxHeight) { this->_maxHeight = maxHeight; };

    int getMaxGenHeight() const { return this->_maxGenHeight; };
    void setMaxGenHeight(const double maxGenHeight) { this->_maxGenHeight = maxGenHeight; };

    int getWidth() const { return this->_width; };
    void setWidth(const int width) { this->_width = width; };

    int getSquare() const { return this->_square; };
    void setSquare(const int square) { this->_square = square; };

    int getLenght() const { return this->_lenght; };
    void setLenght(const int lenght) { this->_width = lenght; };

    int getCols() const { return this->_cols; };
    int getRows() const { return this->_rows; };

    vector<Operation> &getOperations() { return this->_operations; };

    Matrix<double> &getHeightMap() { return this->_heightMap; };

    Matrix<QVector3D> &getScreenHeightMap() { return this->_screenHeightMap; };

    Matrix<double> &getWithoutWaterHeightMap() { return this->_withoutWaterHeightMap; };

    Matrix<pair<QVector3D, QVector3D>> &getNormalMap() { return this->_normalMap; };

    Matrix<QVector3D> &getNormalVertexMap() { return this->_normalVertexMap; };

    Matrix<double> &getIntensityVertexMap() { return this->_intensityVertexMap; };
};

#endif
