#ifndef DIAMOND_SQUARE_H
#define DIAMOND_SQUARE_H

#include "matrix.h"
#include "point.h"
#include <algorithm>
#include <random>

class DiamondSquare
{
public:
    DiamondSquare(int lenght, int width, Matrix<Point<double>> &height_map);

    Matrix<Point<double>> &generate_heights();

private:
    int _lenght, _width;
    Matrix<Point<double>> _points;

public:
    static int random(const int low, const int high);
};

#endif
