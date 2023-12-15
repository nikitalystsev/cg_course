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

class LandscapeManager
{
public:
    static void generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise);
    static void applyOperation(QVector3D &point, vector<Operation> &operations);
    static void changeWaterlevel(Landscape &landscape, int newWaterlevel);
    static void calcNormalForEachPlane(Landscape &landscape);
    static void calcNormalForEachVertex(Landscape &landscape);
    static void calcIntensityForEachVertex(Landscape &landscape, Light &light);
    static void updateLandscape(Landscape &landscape, PerlinNoise &paramNoise, Light &light);

    static void rotateLandscape(Landscape &landscape, Rotate &rotate);
    static void moveLandscape(Landscape &landscape, Move &move);
};

#endif // __LANDSCAPE_MANAGER_H__
