#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "point/point3D.h"
#include "vector/vector3D.h"
#include <QGraphicsScene>
#include <QPainter>
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
    Matrix<Point3D<double>> _heightMap;         // карта высот
    Matrix<double> _withoutWaterHeightMap = {}; // карта высот без учета уровня воды
    int _rows, _cols;                           // размеры ландшафтных матриц
    int _width, _lenght;                        // длина и ширина карты высот (в количестве полигонов)
    double _waterlevel;                         // уровень воды
    double _maxHeight = 0;                      // максимаьная сгенерирова

    Matrix<pair<Vector3D<double>, Vector3D<double>>> _normalMap; // матрица векторов внешней нормали к каждой из граней
    Matrix<Vector3D<double>> _normalVertexMap;                   // матрица векторов нормалей для каждой вершины
    Matrix<double> _intensityVertexMap;                          // расчет интенсивности в каждой вершине

    //    void _shiftPointToOrigin(Point3D<double> &point);
    //    void _shiftPointBackToOrigin(Point3D<double> &point);

public:
    Landscape();
    Landscape(const int width, const int lenght, const int waterlevel);
    explicit Landscape(const Landscape &other);
    Landscape(Landscape &&other) noexcept;
    ~Landscape();

    Landscape &operator=(const Landscape &other);
    Landscape &operator=(Landscape &&other) noexcept;

    void resize(const int width, const int lenght);

    double getWaterlevel() const;
    void updateWaterlevel(const double waterlevel);

    int getMaxHeight() const;
    void setMaxHeight(const double maxHeight);

    int getWidth() const;
    void setWidth(const int width);

    int getLenght() const;
    void setLenght(const int lenght);

    Matrix<Point3D<double>> &getHeightMap();

    Matrix<double> &getWithoutWaterHeightMap();

    Matrix<pair<Vector3D<double>, Vector3D<double>>> &getNormalMap();

    Matrix<Vector3D<double>> &getNormalVertexMap();

    Matrix<double> &getIntensityVertexMap();

public:
    static const int square = 3;
};

#endif
