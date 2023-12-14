#include "../inc/landscapeManager.h"

void LandscapeManager::generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise)
{
    int rows = landscape.getWidth() + 1;
    int cols = landscape.getLenght() + 1;
    int currWaterlevel = landscape.getWaterlevel();
    int maxHeight = 0;

    Matrix<QVector3D> &map = landscape.getHeightMap();
    Matrix<double> &withoutWaterMap = landscape.getWithoutWaterHeightMap();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double nx = i / (double)rows - 1 - 0.5;
            double ny = j / (double)cols - 1 - 0.5;

            double height = paramNoise.generateNoise(nx, ny);

            height *= 1000;

            if (height > maxHeight)
                maxHeight = height;

            if (height < currWaterlevel)
                map[i][j] = QVector3D(i * landscape.square, j * landscape.square, currWaterlevel);
            else
                map[i][j] = QVector3D(i * landscape.square, j * landscape.square, height);

            withoutWaterMap[i][j] = height;
        }

    landscape.setMaxHeight(maxHeight);
}

void LandscapeManager::calcNormalForEachPlane(Landscape &landscape)
{
    int width = landscape.getWidth();
    int lenght = landscape.getLenght();

    Matrix<QVector3D> &map = landscape.getHeightMap();
    Matrix<pair<QVector3D, QVector3D>> &normalMap = landscape.getNormalMap();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < lenght; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(map[i][j], map[i + 1][j], map[i + 1][j + 1]);
            Plane plane2(map[i][j], map[i + 1][j + 1], map[i][j + 1]);

            // получаем вектора внешних нормалей к граням
            QVector3D normalPlane1(plane1.getA(), plane1.getB(), plane1.getC());
            QVector3D normalPlane2(plane2.getA(), plane2.getB(), plane2.getC());

            normalPlane1.normalize();
            normalPlane2.normalize();

            pair<QVector3D, QVector3D> normals(normalPlane1, normalPlane2);

            normalMap[i][j] = normals;
        }
}

void LandscapeManager::calcNormalForEachVertex(Landscape &landscape)
{
    int rows = landscape.getWidth() + 1;
    int cols = landscape.getLenght() + 1;

    Matrix<pair<QVector3D, QVector3D>> &normalMap = landscape.getNormalMap();
    Matrix<QVector3D> &normalVertexMap = landscape.getNormalVertexMap();

    // идем по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            QVector3D avgNormal;
            // левая верхняя и правая нижняя вершины
            if ((i == 0 && j == 0) || (i == rows - 1 && j == cols - 1))
            {
                if (i == 0 && j == 0) // левая верхняя
                {
                    QVector3D normal1(normalMap[i][j].first);
                    QVector3D normal2(normalMap[i][j].second);
                    avgNormal = (normal1 + normal2) / 2;
                }
                else // правая нижняя
                {
                    QVector3D normal1(normalMap[i - 1][j - 1].first);
                    QVector3D normal2(normalMap[i - 1][j - 1].second);
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
                    QVector3D normal1(normalMap[i][j].first);
                    QVector3D normal2(normalMap[i][j].second);
                    QVector3D normal3(normalMap[i][j - 1].first);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
                else // первый столбец
                {
                    QVector3D normal1(normalMap[i][j].first);
                    QVector3D normal2(normalMap[i][j].second);
                    QVector3D normal3(normalMap[i - 1][j].second);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
            }
            else // внутренние точки
            {
                QVector3D normal1(normalMap[i - 1][j - 1].first);
                QVector3D normal2(normalMap[i - 1][j - 1].second);
                QVector3D normal3(normalMap[i][j].first);
                QVector3D normal4(normalMap[i][j].second);
                QVector3D normal5(normalMap[i][j - 1].first);
                QVector3D normal6(normalMap[i - 1][j].second);
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
    Matrix<QVector3D> &normalVertexMap = landscape.getNormalVertexMap();
    Matrix<QVector3D> &map = landscape.getHeightMap();

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            // получили вектор направления света
            QVector3D direction = LightManager::caclDirectionVector(light, map[i][j]);
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
    generateHeightMap(landscape, paramNoise);
    calcNormalForEachPlane(landscape);
    calcNormalForEachVertex(landscape);
    calcIntensityForEachVertex(landscape, light);
}

void LandscapeManager::rotateLandscape(Landscape &landscape, rotate_t &angles)
{
    Matrix<QVector3D> &heightMap = landscape.getHeightMap();

    int rows = heightMap.size();
    int cols = heightMap[0].size();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            Transform::rotateByX(heightMap[i][j], angles.xAngle);
            Transform::rotateByY(heightMap[i][j], angles.xAngle);
            Transform::rotateByZ(heightMap[i][j], angles.xAngle);
        }
}

void LandscapeManager::moveLandscape(Landscape &landscape, move_t &move)
{
    Matrix<QVector3D> &heightMap = landscape.getHeightMap();

    int rows = heightMap.size();
    int cols = heightMap[0].size();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            heightMap[i][j].setX(heightMap[i][j].x() + move.dx);
            heightMap[i][j].setY(heightMap[i][j].y() + move.dy);
            heightMap[i][j].setZ(heightMap[i][j].z() + move.dz);
        }
}
