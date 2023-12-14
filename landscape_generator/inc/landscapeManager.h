#ifndef __LANDSCAPE_MANAGER_H__
#define __LANDSCAPE_MANAGER_H__

#include "landscape.h"
#include "light.h"
#include "lightManager.h"
#include "perlinNoise.h"
#include "plane.h"
#include "transform.h"

template <typename T>
using vector = std::vector<T>;

struct rotate_t
{
    double xAngle;
    double yAngle;
    double zAngle;
};

struct move_t
{
    double dx;
    double dy;
    double dz;
};

class LandscapeManager
{
public:
    static void generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise);
    static void calcNormalForEachPlane(Landscape &landscape);
    static void calcNormalForEachVertex(Landscape &landscape);
    static void calcIntensityForEachVertex(Landscape &landscape, Light &light);
    static void updateLandscape(Landscape &landscape, PerlinNoise &paramNoise, Light &light);

    static void rotateLandscape(Landscape &landscape, rotate_t &angles);
    static void moveLandscape(Landscape &landscape, move_t &move);
};

#endif // __LANDSCAPE_MANAGER_H__
