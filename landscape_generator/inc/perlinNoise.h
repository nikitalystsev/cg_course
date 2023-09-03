#ifndef PERLIN_NOISE
#define PERLIN_NOISE

#include <vector>

class PerlinNoise
{
private:
    int _seed;           // начальное значние для гпсч
    int _octaves;        // число повторений шумов
    double _frequency;   // частота
    double _lacunarity;  // постоянство - параметр, отвечающий за изменение частоты
    double _amplitude;   // амплитуда
    double _persistence; // стойкость - параметр, отвечающий за изменение амплидуты

    std::vector<int> _ptable;

    void _createPtable();
    double _noise(double x, double y);

public:
    PerlinNoise(const int seed,
                const int octaves,
                const double frequency,
                const double lacunarity,
                const double amplitude,
                const double persistence);
    ~PerlinNoise() = default;
    double generateHeight(const double x, const double y);

public:
    static double smooth(const double t);
    static double lerp(const double a, const double b, const double t);
    static double grad(const int hash, const double x, const double y, const double z);
};

#endif
