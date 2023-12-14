#include "../inc/landscapeManager.h"

void LandscapeManager::generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise)
{
    std::cout << "[B] generateHeightMap" << std::endl;

    int rows = landscape.getRows();
    int cols = landscape.getCols();

    int a = -((landscape.getWidth() * landscape.square) / 2);
    int b = -((landscape.getLenght() * landscape.square) / 2);
    int c = -a;
    int d = -b;

    int currWaterlevel = landscape.getWaterlevel();
    int maxHeight = 0;

    Matrix<QVector3D> &heightMap = landscape.getHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
    Matrix<double> &withoutWaterMap = landscape.getWithoutWaterHeightMap();

    for (int i = 0, m = a; i < rows && m <= c; ++i, m += landscape.square)
        for (int j = 0, n = b; j < cols && n <= d; ++j, n += landscape.square)
        {
            double nx = i / (double)rows - 1 - 0.5;
            double ny = j / (double)cols - 1 - 0.5;

            double height = paramNoise.generateNoise(nx, ny);

            height *= 1000;

            if (height > maxHeight)
                maxHeight = height;

            if (height < currWaterlevel)
            {
                heightMap[i][j] = QVector3D(m, n, currWaterlevel);
            }
            else
            {
                heightMap[i][j] = QVector3D(m, n, height);
            }

            screenHeightMap[i][j] = heightMap[i][j];
            withoutWaterMap[i][j] = height;
        }

    landscape.setMaxHeight(maxHeight);
}

void LandscapeManager::calcNormalForEachPlane(Landscape &landscape)
{
    std::cout << "[B] calcNormalForEachPlane" << std::endl;

    int width = landscape.getWidth();
    int lenght = landscape.getLenght();

    //    Matrix<QVector3D> &map = landscape.getHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
    Matrix<pair<QVector3D, QVector3D>> &normalMap = landscape.getNormalMap();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < lenght; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenHeightMap[i][j], screenHeightMap[i + 1][j], screenHeightMap[i + 1][j + 1]);
            Plane plane2(screenHeightMap[i][j], screenHeightMap[i + 1][j + 1], screenHeightMap[i][j + 1]);

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
    std::cout << "[B] calcNormalForEachVertex" << std::endl;

    int rows = landscape.getRows();
    int cols = landscape.getCols();

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
    std::cout << "[B] calcIntensityForEachVertex" << std::endl;

    int rows = landscape.getRows();
    int cols = landscape.getCols();

    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();
    Matrix<QVector3D> &normalVertexMap = landscape.getNormalVertexMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            // получили вектор направления света
            QVector3D direction = LightManager::caclDirectionVector(light, screenHeightMap[i][j]);
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
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    for (auto &row : screenHeightMap)
        for (auto &point : row)
        {
            Transform::rotateByX(point, angles.xAngle);
            Transform::rotateByY(point, angles.yAngle);
            Transform::rotateByZ(point, angles.zAngle);
        }
}

void LandscapeManager::moveLandscape(Landscape &landscape, move_t &move)
{
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    for (auto &row : screenHeightMap)
        for (auto &point : row)
        {
            point.setX(point.x() + move.dx);
            point.setY(point.y() + move.dy);
            point.setZ(point.z() + move.dz);
        }
}
