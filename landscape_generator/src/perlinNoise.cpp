#include "../inc/perlinNoise.h"
#include <iostream>
#include <random>

PerlinNoise::PerlinNoise() :
    PerlinNoise(24, 8, 1, 4, 1, 0.25) {}

PerlinNoise::PerlinNoise(const int seed,
                         const int octaves,
                         const double frequency,
                         const double lacunarity,
                         const double amplitude,
                         const double persistence) :
    _seed(seed),
    _octaves(octaves), _frequency(frequency),
    _lacunarity(lacunarity), _amplitude(amplitude), _persistence(persistence)
{
    this->_createPtable();
}

PerlinNoise::PerlinNoise(const PerlinNoise &other)
{
    this->_seed = other._seed;
    this->_octaves = other._octaves;
    this->_frequency = other._frequency;
    this->_lacunarity = other._lacunarity;
    this->_amplitude = other._amplitude;
    this->_persistence = other._persistence;

    this->_ptable = other._ptable;
}

PerlinNoise::PerlinNoise(PerlinNoise &&other) noexcept
{
    this->_seed = other._seed;
    this->_octaves = other._octaves;
    this->_frequency = other._frequency;
    this->_lacunarity = other._lacunarity;
    this->_amplitude = other._amplitude;
    this->_persistence = other._persistence;

    this->_ptable = other._ptable;
}

PerlinNoise &PerlinNoise::operator=(const PerlinNoise &other)
{
    this->_seed = other._seed;
    this->_octaves = other._octaves;
    this->_frequency = other._frequency;
    this->_lacunarity = other._lacunarity;
    this->_amplitude = other._amplitude;
    this->_persistence = other._persistence;

    this->_ptable = other._ptable;

    return *this;
}

PerlinNoise &PerlinNoise::operator=(PerlinNoise &&other) noexcept
{
    this->_seed = other._seed;
    this->_octaves = other._octaves;
    this->_frequency = other._frequency;
    this->_lacunarity = other._lacunarity;
    this->_amplitude = other._amplitude;
    this->_persistence = other._persistence;

    this->_ptable = other._ptable;

    return *this;
}

double PerlinNoise::generateNoise(const double x, const double y)
{
    int octaves = this->_octaves;
    double frequency = this->_frequency;
    double amplitude = this->_amplitude;

    double result = 0;
    double sum_ampl = 0; // необходимо для нормализации результата

    for (; octaves > 0; octaves--)
    {
        result += this->_noise(x * frequency, y * frequency) * amplitude;
        sum_ampl += amplitude;

        amplitude *= this->_persistence;
        frequency *= this->_lacunarity;
    }

    return result / sum_ampl;
}

void PerlinNoise::_createPtable()
{
    for (int i = 0; i < 256; ++i)
        this->_ptable.push_back(i);

    shuffle(this->_ptable.begin(),
            this->_ptable.end(),
            std::default_random_engine(this->_seed));

    for (int i = 0; i < 256; ++i)
        this->_ptable.push_back(this->_ptable[i]);
}

double PerlinNoise::_noise(double x, double y)
{
    int xi = (int)(floor(x)) & 255; // = % 256
    int yi = (int)(floor(y)) & 255; // для того, чтобы не выйти за пределы массива

    x -= floor(x);
    y -= floor(y);

    double u = smooth(x);
    double v = smooth(y);

    double tl = grad(this->_ptable[this->_ptable[xi] + yi], x, y, 0);
    double tr = grad(this->_ptable[this->_ptable[xi] + yi + 1], x, y - 1, 0);
    double bl = grad(this->_ptable[this->_ptable[xi + 1] + yi + 1], x - 1, y - 1, 0);
    double br = grad(this->_ptable[this->_ptable[xi + 1] + yi], x - 1, y, 0);

    double x1 = this->lerp(tl, br, u);
    double x2 = this->lerp(tr, bl, u);

    double res = this->lerp(0, 1, this->lerp(x1, x2, v));

    return res;
}

double PerlinNoise::smooth(const double t) // функция сглаживания
{
    return t * t * t * (t * (6 * t - 15) + 10);
}

double PerlinNoise::lerp(const double a, const double b, const double t) // функция линейной интерполяции
{
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z)
{
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x
                                              : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
