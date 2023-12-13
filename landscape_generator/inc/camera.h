#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "transform.h"
#include "vector/vector2D.h"
#include "vector/vector3D.h"
#include <vector>

// это временно, пока не разберусь с устройством камеры

// Ближняя граница области отсечения
#define CAMERA_NEAR 0.1f
// Дальняя граница области отсечения
#define CAMERA_FAR 1000.0f
// Стандартный угол вертикального обзора
#define CAMERA_FOVy 60.0f
// Вектор, задающий верх для камеры
#define CAMERA_UP_VECTOR Vector3D<double>(0, 1, 0)

template <typename T>
using vector = std::vector<T>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

class Camera
{
private:
    Vector3D<double> _position;     // местоположение камеры
    Vector2D<double> _currRotation; // вектор текущего поворота камеры
    Vector3D<double> _target;       // направление камеры (точка, куда смотрит камера)
    Vector3D<double> _up;           // вектор, указывающий верх для камеры
    Matrix<double> _view;           // матрица вида (местоположения) камеры
    Matrix<double> _projection;     // матрица проекции камеры

    void _calcTarget();

public:
    Camera(double aspect, Vector3D<double> position, Vector2D<double> rotation, double fovy = CAMERA_FOVy); // Конструктор камеры с проекцией перспективы

    void rotate(const Vector2D<double> &rotation);         // поворот камеры
    void move(const Vector3D<double> &movement);           // перемещение камеры
    void lookAt();                                         // расчет матрицы вида
    void calcProjectionMatrix(double aspect, double fovy); // расчет матрицы перспективной проекции

    Matrix<double> getView() const;
    Matrix<double> getProjection() const;
};

#endif // __CAMERA_H__
