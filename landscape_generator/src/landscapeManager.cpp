#include "../inc/landscapeManager.h"

void LandscapeManager::generateHeightMap(Landscape &landscape, PerlinNoise &paramNoise)
{
    //    std::cout << "[INFO] call generateHeightMap" << std::endl;

    int rows = landscape.getRows();
    int cols = landscape.getCols();
    int square = landscape.getSquare();

    int a = -((landscape.getWidth() * square) / 2);
    int b = -((landscape.getLenght() * square) / 2);
    int c = -a;
    int d = -b;

    int currWaterlevel = landscape.getWaterlevel();
    int maxGenHeight = landscape.getMaxGenHeight();
    int maxHeight = 0;

    Matrix<double> &heightMap = landscape.getHeightMap();
    Matrix<double> &withoutWaterHeightMap = landscape.getWithoutWaterHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    vector<Operation> &operations = landscape.getOperations();

    //#pragma omp parallel for

    for (int i = 0, m = a; i < rows && m <= c; ++i, m += square)
        for (int j = 0, n = b; j < cols && n <= d; ++j, n += square)
        {
            double nx = i / (double)rows - 1 - 0.5;
            double ny = j / (double)cols - 1 - 0.5;

            double height = paramNoise.generateNoise(nx, ny);

            height *= maxGenHeight;

            if (height > maxHeight)
                maxHeight = height;

            if (height < currWaterlevel)
                screenHeightMap[i][j] = QVector3D(m, n, currWaterlevel);
            else
                screenHeightMap[i][j] = QVector3D(m, n, height);

            heightMap[i][j] = screenHeightMap[i][j].z();
            withoutWaterHeightMap[i][j] = height;

            applyOperation(screenHeightMap[i][j], operations);
        }

    landscape.setMaxHeight(maxHeight);
}

void LandscapeManager::changeWaterlevel(Landscape &landscape, int newWaterlevel)
{
    int rows = landscape.getRows();
    int cols = landscape.getCols();
    int square = landscape.getSquare();

    Matrix<double> &heightMap = landscape.getHeightMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
    Matrix<double> &withoutWaterHeightMap = landscape.getWithoutWaterHeightMap();

    vector<Operation> &operations = landscape.getOperations();

    int a = -((landscape.getWidth() * square) / 2);
    int b = -((landscape.getLenght() * square) / 2);
    int c = -a;
    int d = -b;

    //#pragma omp parallel for

    for (int i = 0, m = a; i < rows && m <= c; ++i, m += square)
        for (int j = 0, n = b; j < cols && n <= d; ++j, n += square)
        {
            if (withoutWaterHeightMap[i][j] < newWaterlevel)
                screenHeightMap[i][j] = QVector3D(m, n, newWaterlevel);
            else
                screenHeightMap[i][j] = QVector3D(m, n, withoutWaterHeightMap[i][j]);

            heightMap[i][j] = screenHeightMap[i][j].z();

            applyOperation(screenHeightMap[i][j], operations);
        }

    landscape.setWaterlevel(newWaterlevel);
}

void LandscapeManager::applyOperation(QVector3D &point, vector<Operation> &operations)
{
    for (auto &operation : operations)
        switch (operation.operIndex)
        {
        case 0:
            switch (operation.axisIndex)
            {
            case 0:
                point.setX(point.x() + operation.value);
                break;
            case 1:
                point.setY(point.y() + operation.value);
                break;
            case 2:
                point.setZ(point.z() + operation.value);
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (operation.axisIndex)
            {
            case 0:
                point.setX(operation.value * point.x() + (1 - operation.value) * 0);
                break;
            case 1:
                point.setY(operation.value * point.y() + (1 - operation.value) * 0);
                break;
            case 2:
                point.setZ(operation.value * point.z() + (1 - operation.value) * 0);
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (operation.axisIndex)
            {
            case 0:
                Transform::rotateByX(point, operation.value);
                break;
            case 1:
                Transform::rotateByY(point, operation.value);
                break;
            case 2:
                Transform::rotateByZ(point, operation.value);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
}

void LandscapeManager::calcNormalForEachPlane(Landscape &landscape)
{
    //    std::cout << "[INFO] call calcNormalForEachPlane" << std::endl;

    int width = landscape.getWidth();
    int lenght = landscape.getLenght();

    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();
    Matrix<pair<QVector3D, QVector3D>> &normalMap = landscape.getNormalMap();
    //#pragma omp parallel for
    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < width; ++i)
        //#pragma omp parallel for
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
    //    std::cout << "[INFO] call calcNormalForEachVertex" << std::endl;

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
    //    std::cout << "[INFO] call calcIntensityForEachVertex" << std::endl;

    int rows = landscape.getRows();
    int cols = landscape.getCols();

    Matrix<double> &intensityVertexMap = landscape.getIntensityVertexMap();
    Matrix<QVector3D> &normalVertexMap = landscape.getNormalVertexMap();
    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

// цикл по всем вершинам ландшафтной сетки
#pragma omp parallel for
    for (int i = 0; i < rows; ++i)
#pragma omp parallel for
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
    //    std::cout << "[INFO] call updateLandscape" << std::endl;

    generateHeightMap(landscape, paramNoise);
    calcNormalForEachPlane(landscape);
    calcNormalForEachVertex(landscape);
    calcIntensityForEachVertex(landscape, light);
}

void LandscapeManager::rotateLandscape(Landscape &landscape, Rotate &rotate)
{
    //    std::cout << "[INFO] call rotateLandscape" << std::endl;

    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    vector<Operation> &operations = landscape.getOperations();

    for (auto &row : screenHeightMap)
        for (auto &point : row)
            Transform::rotate(point, rotate);

    Operation newOperation;

    if (rotate.xAngle != 0)
    {
        newOperation = {2, 0, (double)rotate.xAngle};
        operations.push_back(newOperation);
    }
    if (rotate.yAngle != 0)
    {
        newOperation = {2, 1, (double)rotate.yAngle};
        operations.push_back(newOperation);
    }

    if (rotate.zAngle != 0)
    {
        newOperation = {2, 2, (double)rotate.zAngle};
        operations.push_back(newOperation);
    }
}

void LandscapeManager::moveLandscape(Landscape &landscape, Move &move)
{
    //    std::cout << "[INFO] call moveLandscape" << std::endl;

    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    vector<Operation> &operations = landscape.getOperations();

    for (auto &row : screenHeightMap)
        for (auto &point : row)
            Transform::move(point, move);

    Operation newOperation;

    if (move.dx != 0)
    {
        newOperation = {0, 0, (double)move.dx};
        operations.push_back(newOperation);
    }
    if (move.dy != 0)
    {
        newOperation = {0, 1, (double)move.dy};
        operations.push_back(newOperation);
    }

    if (move.dz != 0)
    {
        newOperation = {0, 2, (double)move.dz};
        operations.push_back(newOperation);
    }
}

void LandscapeManager::scaleLandscape(Landscape &landscape, Scale &scale)
{
    //    std::cout << "[INFO] call scaleLandscape" << std::endl;

    Matrix<QVector3D> &screenHeightMap = landscape.getScreenHeightMap();

    vector<Operation> &operations = landscape.getOperations();

    for (auto &row : screenHeightMap)
        for (auto &point : row)
            Transform::scale(point, scale);

    Operation newOperation;

    if (scale.kx != 0)
    {
        newOperation = {1, 0, scale.kx};
        operations.push_back(newOperation);
    }
    if (scale.ky != 0)
    {
        newOperation = {1, 1, scale.ky};
        operations.push_back(newOperation);
    }
    if (scale.kz != 0)
    {
        newOperation = {1, 2, scale.kz};
        operations.push_back(newOperation);
    }
}
