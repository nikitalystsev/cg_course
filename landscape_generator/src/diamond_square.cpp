#include "../inc/diamond_square.h"

DiamondSquare::DiamondSquare(int lenght, int width, Matrix<Point<double>> &height_map) :
    _lenght(lenght), _width(width), _points(height_map)
{
}

Matrix<Point<double>> &DiamondSquare::generate_heights()
{
    srand(time(NULL));
    double rand = this->random(0, 256);

    this->_points[0][0].z(rand);
    this->_points[this->_width - 1][0].z(rand);
    this->_points[0][this->_lenght - 1].z(rand);
    this->_points[this->_width - 1][this->_lenght - 1].z(rand);

    int randomness = 128;
    int tileWidth = this->_width - 1;

    while (tileWidth > 1)
    {
        int halfSide = tileWidth / 2;

        for (int x = 0; x < this->_width - 1; x += tileWidth)
            for (int y = 0; y < this->_width - 1; y += tileWidth)
            {
                int avg = this->_points[x][y].z() +
                          this->_points[x + tileWidth][y].z() +
                          this->_points[x][y + tileWidth].z() +
                          this->_points[x + tileWidth][y + tileWidth].z();

                avg /= 4;
                avg += this->random(-randomness, randomness);

                this->_points[x + halfSide][y + halfSide].z(avg);
            }

        for (int x = 0; x < this->_width - 1; x += halfSide)
            for (int y = (x + halfSide) % tileWidth; y < this->_lenght - 1; y += tileWidth)
            {
                int avg = this->_points[(x - halfSide + this->_lenght - 1) % (this->_lenght - 1)][y].z() +
                          this->_points[(x + halfSide) % (this->_lenght - 1)][y].z() +
                          this->_points[x][(y + halfSide) % (this->_lenght - 1)].z() +
                          this->_points[x][(y - halfSide + this->_lenght - 1) % (this->_lenght - 1)].z();

                avg /= 4.0;
                avg += this->random(-randomness, randomness);

                this->_points[x][y].z(avg);

                if (x == 0)
                    this->_points[this->_lenght - 1][y].z(avg);
                if (y == 0)
                    this->_points[x][this->_lenght - 1].z(avg);
            }

        randomness = std::max(randomness / 2, 1);
        tileWidth /= 2;
    }

    return this->_points;
}

int DiamondSquare::random(const int low, const int high)
{
    return low + rand() % (high - low + 1);
}
