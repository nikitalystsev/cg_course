#include "../inc/landscapeManager.h"

void LandscapeManager::generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise)
{
    int rows = landscape.getWidth() + 1;
    int cols = landscape.getLenght() + 1;
    int currWaterlevel = landscape.getWaterlevel();
    int maxHeight = landscape.getMaxHeight();

    Matrix<Point3D<double>> &map = landscape.getMap();
    Matrix<double> &withoutWaterMap = landscape.getWithoutWaterMap();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double nx = i / (double)rows - 1 - 0.5;
            double ny = j / (double)cols - 1 - 0.5;

            double height = paramNoise.generateNoise(nx, ny);

            if (height > maxHeight)
                maxHeight = height;

            height *= 1000;

            if (height < currWaterlevel)
                map[i][j].set(i * 5, j * 5, currWaterlevel);
            else
                map[i][j].set(i * 5, j * 5, height);

            withoutWaterMap[i][j] = height;
        }

    landscape.setMaxHeight(maxHeight);
}

void LandscapeManager::calcNormalForEachPlane(Landscape &landscape)
{
    int width = landscape.getWidth();
    int lenght = landscape.getLenght();

    Matrix<Point3D<double>> &map = landscape.getMap();
    Matrix<pair<Vector3D<double>, Vector3D<double>>> &normalMap = landscape.getNormalMap();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < lenght; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(map[i][j], map[i + 1][j], map[i + 1][j + 1]);
            Plane plane2(map[i][j], map[i + 1][j + 1], map[i][j + 1]);

            // получаем вектора внешних нормалей к граням
            Vector3D<double> normalPlane1(plane1.getA(), plane1.getB(), plane1.getC());
            Vector3D<double> normalPlane2(plane2.getA(), plane2.getB(), plane2.getC());

            normalPlane1.normalize();
            normalPlane2.normalize();

            pair<Vector3D<double>, Vector3D<double>> normals(normalPlane1, normalPlane2);

            normalMap[i][j] = normals;
        }
}

void LandscapeManager::calcNormalForEachVertex(Landscape &landscape)
{
    int rows = landscape.getWidth() + 1;
    int cols = landscape.getLenght() + 1;

    Matrix<pair<Vector3D<double>, Vector3D<double>>> &normalMap = landscape.getNormalMap();
    Matrix<Vector3D<double>> &normalVertexMap = landscape.getNormalVertexMap();

    // идем по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            Vector3D<double> avgNormal;
            // левая верхняя и правая нижняя вершины
            if ((i == 0 && j == 0) || (i == rows - 1 && j == cols - 1))
            {
                if (i == 0 && j == 0) // левая верхняя
                {
                    Vector3D<double> normal1(normalMap[i][j].first);
                    Vector3D<double> normal2(normalMap[i][j].second);
                    avgNormal = (normal1 + normal2) / 2;
                }
                else // правая нижняя
                {
                    Vector3D<double> normal1(normalMap[i - 1][j - 1].first);
                    Vector3D<double> normal2(normalMap[i - 1][j - 1].second);
                    avgNormal = (normal1 + normal2) / 2;
                }
            }
            // правая верхняя и левая нижняя вершина
            else if ((i == 0 && j == cols - 1) || (i == rows - 1 && j == 0))
            {
                if (i == 0 && j == cols - 1) // правая верхняя
                    avgNormal = normalMap[i][j - 1].first;
                else // левая верхняя
                    avgNormal = normalMap[i - 1][j].second;
            }
            // вершины по краям карты (с 3-мя примыкающими гранями)
            else if (i == 0 || j == 0)
            {
                if (i == 0) // первая строка
                {
                    Vector3D<double> normal1(normalMap[i][j].first);
                    Vector3D<double> normal2(normalMap[i][j].second);
                    Vector3D<double> normal3(normalMap[i][j - 1].first);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
                else // первый столбец
                {
                    Vector3D<double> normal1(normalMap[i][j].first);
                    Vector3D<double> normal2(normalMap[i][j].second);
                    Vector3D<double> normal3(normalMap[i - 1][j].second);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
            }
            else // внутренние точки
            {
                Vector3D<double> normal1(normalMap[i - 1][j - 1].first);
                Vector3D<double> normal2(normalMap[i - 1][j - 1].second);
                Vector3D<double> normal3(normalMap[i][j].first);
                Vector3D<double> normal4(normalMap[i][j].second);
                Vector3D<double> normal5(normalMap[i][j - 1].first);
                Vector3D<double> normal6(normalMap[i - 1][j].second);
                avgNormal = (normal1 + normal2 + normal3 + normal4 + normal5 + normal6) / 6;
            }
            normalVertexMap[i][j] = avgNormal;
        }
}

void LandscapeManager::calcIntensityForEachVertex(Landscape &landscape, Light &light)
{
    int rows = landscape.getWidth() + 1;
    int cols = landscape.getLenght() + 1;

    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();
    Matrix<Vector3D<double>> &normalVertexMap = landscape.getNormalVertexMap();
    Matrix<Point3D<double>> &map = landscape.getMap();

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            // получили вектор направления света
            Vector3D<double> direction = LightManager::caclDirectionVector(light, map[i][j]);
            // // нормализуем вектора, чтобы были единичной длины
            direction.normalize();
            normalVertexMap[i][j].normalize();

            // вот она, интенсивность в вершине
            intensityVertexMap[i][j] = LightManager::caclIntensityAtVertex(light,
                                                                           direction,
                                                                           normalVertexMap[i][j]);
        }
}

void LandscapeManager::updateLandscape(Landscape &landscape, PerlinNoise &paramNoise, Light &light)
{
    LandscapeManager::generateHeightMap(landscape, paramNoise);
    LandscapeManager::calcNormalForEachPlane(landscape);
    LandscapeManager::calcNormalForEachVertex(landscape);
    LandscapeManager::calcIntensityForEachVertex(landscape, light);
}
