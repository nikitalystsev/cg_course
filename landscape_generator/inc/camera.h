#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "transform.h"
#include "vector/vector3D.h"
#include <vector>

// это временно, пока не разберусь с устройством камеры

// Ближняя граница области отсечения
#define CAMERA_NEAR 0.1f
// Дальняя граница области отсечения
#define CAMERA_FAR 1000.0f
// Стандартный угол вертикального обзора
#define CAMERA_FOVy 60.0f

template <typename T>
using vector = std::vector<T>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

class Camera
{
private:
    Vector3D<double> _position; // местоположение камеры
    Vector3D<double> _target;   // направление камеры
    Vector3D<double> _up;       // вектор, указывающий верх для камеры

    Matrix<double> _view;       // матрица вида (местоположения) камеры
    Matrix<double> _projection; // матрица проекции камеры

    Matrix<double> lookAt();               // расчет матрицы вида
    Matrix<double> calcProjectionMatrix(); // расчет матрицы перспективной проекции
};

#endif // __CAMERA_H__
