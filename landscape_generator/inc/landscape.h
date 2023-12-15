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

struct Operation
{
    int operIndex; // индекс выполняемой операции (0, 1, 2)
    int axisIndex; // индекс оси, на которой выполнялась операция (0, 1, 2)
    int value;     //  значение
};

class Landscape
{
private:
    Matrix<QVector3D> _heightMap;               // карта высот
    Matrix<QVector3D> _screenHeightMap;         // карта высот для вывода на экран
    Matrix<double> _withoutWaterHeightMap = {}; // карта высот без учета уровня воды
    int _rows, _cols;                           // размеры ландшафтных матриц
    int _width, _lenght;                        // длина и ширина карты высот (в количестве полигонов)
    double _waterlevel;                         // уровень воды
    double _maxHeight = 0;                      // максимальная сгенерированная высота

    Matrix<pair<QVector3D, QVector3D>> _normalMap; // матрица векторов внешней нормали к каждой из граней
    Matrix<QVector3D> _normalVertexMap;            // матрица векторов нормалей для каждой вершины
    Matrix<double> _intensityVertexMap;            // матрица интенсивности в каждой вершине

    vector<Operation> _operations; // цепочка преобразований, выполненных над ландшафтом

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

    double getWaterlevel() const;
    void setWaterlevel(const double waterlevel);

    int getMaxHeight() const;
    void setMaxHeight(const double maxHeight);

    int getWidth() const;
    void setWidth(const int width);

    int getLenght() const;
    void setLenght(const int lenght);

    int getCols() const;
    int getRows() const;

    vector<Operation> &getOperations();

    Matrix<QVector3D> &getHeightMap();

    Matrix<QVector3D> &getScreenHeightMap();

    Matrix<double> &getWithoutWaterHeightMap();

    Matrix<pair<QVector3D, QVector3D>> &getNormalMap();

    Matrix<QVector3D> &getNormalVertexMap();

    Matrix<double> &getIntensityVertexMap();

public:
    static const int square = 3;
};

#endif
