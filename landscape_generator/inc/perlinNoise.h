#ifndef PERLIN_NOISE
#define PERLIN_NOISE

#include <vector>

template <typename T>
using vector = std::vector<T>;

class PerlinNoise
{
private:
    int _seed;           // начальное значние для гпсч
    int _octaves;        // число повторений шумов
    double _frequency;   // частота
    double _lacunarity;  // постоянство - параметр, отвечающий за изменение частоты
    double _amplitude;   // амплитуда
    double _persistence; // стойкость - параметр, отвечающий за изменение амплидуты

    int _ptable[512];

    void _formPtableBySeed();
    double _noise(double x, double y);

public:
    PerlinNoise();
    PerlinNoise(const int seed,
                const int octaves,
                const double frequency,
                const double lacunarity,
                const double amplitude,
                const double persistence);
    explicit PerlinNoise(const PerlinNoise &other);
    PerlinNoise(PerlinNoise &&other) noexcept;
    ~PerlinNoise() = default;

    PerlinNoise &operator=(const PerlinNoise &other);
    PerlinNoise &operator=(PerlinNoise &&other) noexcept;

    double generateNoise(const double x, const double y);

    void setSeed(const int seed) { this->_seed = seed, this->_formPtableBySeed(); }
    void setOctaves(const int octaves) { this->_octaves = octaves; }
    void setFrequency(const double frequency) { this->_frequency = frequency; }
    void setLacunarity(const double lacunarity) { this->_lacunarity = lacunarity; }
    void setAmplitude(const double amplitude) { this->_amplitude = amplitude; }
    void setPersistense(const double persistence) { this->_persistence = persistence; }

public:
    static double smooth(const double t);
    static double lerp(const double a, const double b, const double t);
    static double grad(const int hash, const double x, const double y, const double z);
};

#endif
