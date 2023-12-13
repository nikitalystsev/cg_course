#ifndef __LANDSCAPE_MANAGER_H__
#define __LANDSCAPE_MANAGER_H__

#include "landscape.h"
#include "light.h"
#include "lightManager.h"
#include "perlinNoise.h"
#include "vector/vector3D.h"

class LandscapeManager
{
public:
    static void generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise);
    static void calcNormalForEachPlane(Landscape &landscape);
    static void calcNormalForEachVertex(Landscape &landscape);
    static void calcIntensityForEachVertex(Landscape &landscape, Light &light);

    static void updateLandscape(Landscape &landscape, PerlinNoise &paramNoise, Light &light);
};

#endif // __LANDSCAPE_MANAGER_H__
