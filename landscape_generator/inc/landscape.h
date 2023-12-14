#ifndef LANDSCAPE_H
#define LANDSCAPE_H

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
    Matrix<QVector3D> _heightMap;               // карта высот
    Matrix<QVector3D> _screenHeightMap;         // карта высот для вывода на экран
    Matrix<double> _withoutWaterHeightMap = {}; // карта высот без учета уровня воды
    int _rows, _cols;                           // размеры ландшафтных матриц
    int _width, _lenght;                        // длина и ширина карты высот (в количестве полигонов)
    double _waterlevel;                         // уровень воды
    double _maxHeight = 0;                      // максимальная сгенерированная высота

    Matrix<pair<QVector3D, QVector3D>> _normalMap; // матрица векторов внешней нормали к каждой из граней
    Matrix<QVector3D> _normalVertexMap;            // матрица векторов нормалей для каждой вершины
    Matrix<double> _intensityVertexMap;            // расчет интенсивности в каждой вершине

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

    template <typename T>
    void resizeMatrix(Matrix<T> &matrix, int newWidth, int newLenght);
    void resize(const int width, const int lenght);

    double getWaterlevel() const;
    void updateWaterlevel(const double waterlevel);

    int getMaxHeight() const;
    void setMaxHeight(const double maxHeight);

    int getWidth() const;
    void setWidth(const int width);

    int getLenght() const;
    void setLenght(const int lenght);

    int getCols() const;
    int getRows() const;

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
