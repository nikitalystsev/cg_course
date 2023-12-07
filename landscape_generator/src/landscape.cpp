#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(default_width, default_lenght, default_waterlevel)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _map(width + 1, vector<Point3D<double>>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1)),
    _light(Point3D<int>(500, 500, 600), 1, 0.5)
{
}

Landscape::~Landscape() {}

void Landscape::generateHeightMap()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    PerlinNoise alg(22, 8, 1, 4, 1, 0.25);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            double nx = i / (double)this->_width - 0.5;
            double ny = j / (double)this->_lenght - 0.5;

            double height = alg.generateNoise(nx, ny);

            height *= 1000;

            if (height < _waterlevel)
                this->_map[i][j].set((i + 1) * this->poly_size, (j + 1) * this->poly_size, _waterlevel);
            else
                this->_map[i][j].set((i + 1) * this->poly_size, (j + 1) * this->poly_size, height);
        }
}

void Landscape::draw(QGraphicsScene *scene)
{
    std::cout << "[B] draw" << std::endl;

    Matrix<Point3D<double>> screenMap = this->_mapToScreen();

    this->_calcZBuffer(screenMap);
    this->_calcNormalForEachPlane();
    this->_calcNormalForEachVertex();
    this->_calcIntensityForEachVertex();
    this->_calcFramebuffer(screenMap);
    this->_drawMap(scene);
}

Matrix<Point3D<double>> Landscape::_mapToScreen()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    this->_calcCenterPoint();

    Matrix<Point3D<double>> tmp(this->_map);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->_shiftPointToOrigin(tmp[i][j]);

            Transform::pointToIsometric(tmp[i][j]);

            this->_shiftPointBackToOrigin(tmp[i][j]);
            this->_movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

void Landscape::_drawMap(QGraphicsScene *scene) const
{
    const int width = this->_zBuffer.getWidth();
    const int height = this->_zBuffer.getHeight();

    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);

    QImage image(width, height, QImage::Format_RGB32);
    int r, g, b;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            _zBuffer.getFramebufElem(i, j).getRgb(&r, &g, &b);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
}

void Landscape::_calcZBuffer(const Matrix<Point3D<double>> &screenMap)
{
    std::cout << "[B] _calcZBuffer" << std::endl;

    this->_zBuffer.clean();

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenMap[i][j], screenMap[i + 1][j], screenMap[i + 1][j + 1]);
            Plane plane2(screenMap[i + 1][j + 1], screenMap[i][j + 1], screenMap[i][j]);

            // определяем текущее состояние z-буффера
            _zBuffer.calcZBufferByPlane(plane1);
            _zBuffer.calcZBufferByPlane(plane2);
        }
}

void Landscape::_calcCenterPoint()
{
    //    std::cout << "call _calcCenterPoint" << std::endl;

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    Point3D<double> p1(this->_map[0][0]);
    Point3D<double> p2(this->_map[rows - 1][cols - 1]);

    this->_centerPoint.setX((p1.getX() + p2.getX()) / 2);
    this->_centerPoint.setY((p1.getY() + p2.getY()) / 2);

    double zMin = p1.getZ(), zMax = p1.getZ();

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            if (this->_map[i][j].getZ() < zMin)
                zMin = this->_map[i][j].getZ();
            if (this->_map[i][j].getZ() > zMax)
                zMax = this->_map[i][j].getZ();
        }

    this->_centerPoint.setZ((zMin + zMax) / 2);
}

void Landscape::_shiftPointToOrigin(Point3D<double> &point)
{
    double x = point.getX() - this->_centerPoint.getX();
    double y = point.getY() - this->_centerPoint.getY();
    double z = point.getZ() - this->_centerPoint.getZ();

    point.set(x, y, z);
}

void Landscape::_shiftPointBackToOrigin(Point3D<double> &point)
{
    double x = point.getX() + this->_centerPoint.getX();
    double y = point.getY() + this->_centerPoint.getY();
    double z = point.getZ() + this->_centerPoint.getZ();

    point.set(x, y, z);
}

void Landscape::_movePointToCenter(Point3D<double> &point)
{
    double x = point.getX() + this->_zBuffer.getWidth() / 2 - this->_centerPoint.getX();
    double y = point.getY() + this->_zBuffer.getHeight() / 2 - this->_centerPoint.getY();
    double z = point.getZ();

    point.set(x, y, z);
}

void Landscape::_calcNormalForEachPlane()
{
    std::cout << "[B] _calcNormalForEachPlane" << std::endl;

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            // this->_map[i][j].setZ(500);
            // this->_map[i][j + 1].setZ(375);
            // this->_map[i + 1][j + 1].setZ(625);
            // this->_map[i][j].print();
            // this->_map[i][j + 1].print();
            // this->_map[i + 1][j + 1].print();

            Plane plane2(this->_map[i][j], this->_map[i][j + 1], this->_map[i + 1][j + 1]);

            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(this->_map[i][j], this->_map[i + 1][j], this->_map[i + 1][j + 1]);

            // получаем вектора внешних нормалей к граням
            Vector3D<double> normalPlane1(plane1.getA(), plane1.getB(), plane1.getC());
            Vector3D<double> normalPlane2(plane2.getA(), plane2.getB(), plane2.getC());

            // нормализуем вектора, чтобы были единичной длины
            // normalPlane1.print();
            // normalPlane2.print();

            // normalPlane1.normalize();
            // normalPlane2.normalize();

            pair<Vector3D<double>, Vector3D<double>> normals(normalPlane1, normalPlane2);

            this->_normalMap[i][j] = normals;
        }

    // this->_printPlaneNormals();
}

void Landscape::_printPlaneNormals()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            std::cout << "[=] Первый:" << std::endl;
            this->_normalMap[i][j].first.print();
            std::cout << "[=] Второй:" << std::endl;
            this->_normalMap[i][j].second.print();
        }
}

void Landscape::_calcNormalForEachVertex()
{
    std::cout << "[B] _calcNormalForEachVertex" << std::endl;

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

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
                    Vector3D<double> normal1(this->_normalMap[i][j].first);
                    Vector3D<double> normal2(this->_normalMap[i][j].second);
                    avgNormal = (normal1 + normal2) / 2;
                }
                else // правая нижняя
                {
                    Vector3D<double> normal1(this->_normalMap[i - 1][j - 1].first);
                    Vector3D<double> normal2(this->_normalMap[i - 1][j - 1].second);
                    avgNormal = (normal1 + normal2) / 2;
                }
                this->_normalVertexMap[i][j] = avgNormal;
            }
            // правая верхняя и левая нижняя вершина
            else if ((i == 0 && j == cols - 1) || (i == rows - 1 && j == 0))
            {
                if (i == 0 && j == cols - 1) // правая верхняя
                    this->_normalVertexMap[i][j] = this->_normalMap[i][j - 1].first;
                else // левая верхняя
                    this->_normalVertexMap[i][j] = this->_normalMap[i - 1][j].second;
            }
            // вершины по краям карты (с 3-мя примыкающими гранями)
            else if (i == 0 || j == 0)
            {
                if (i == 0) // первая строка
                {
                    Vector3D<double> normal1(this->_normalMap[i][j].first);
                    Vector3D<double> normal2(this->_normalMap[i][j].second);
                    Vector3D<double> normal3(this->_normalMap[i][j - 1].first);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
                else // первый столбец
                {
                    Vector3D<double> normal1(this->_normalMap[i][j].first);
                    Vector3D<double> normal2(this->_normalMap[i][j].second);
                    Vector3D<double> normal3(this->_normalMap[i - 1][j].second);
                    avgNormal = (normal1 + normal2 + normal3) / 3;
                }
                this->_normalVertexMap[i][j] = avgNormal;
            }
            else // внутренние точки
            {
                Vector3D<double> normal1(this->_normalMap[i - 1][j - 1].first);
                Vector3D<double> normal2(this->_normalMap[i - 1][j - 1].second);
                Vector3D<double> normal3(this->_normalMap[i][j].first);
                Vector3D<double> normal4(this->_normalMap[i][j].second);
                Vector3D<double> normal5(this->_normalMap[i][j - 1].first);
                Vector3D<double> normal6(this->_normalMap[i - 1][j].second);
                avgNormal = (normal1 + normal2 + normal3 + normal4 + normal5 + normal6) / 6;
                this->_normalVertexMap[i][j] = avgNormal;
            }
        }
    // this->_printVertexNormals();
}

void Landscape::_printVertexNormals()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            this->_normalVertexMap[i][j].print();
        }
}

void Landscape::_calcIntensityForEachVertex()
{
    std::cout << "[B] _caclIntensityForEachVertex" << std::endl;

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            // получили вектор направления света
            Vector3D<double> direction = this->_light.caclDirectionVector(this->_map[i][j]);
            // // нормализуем вектора, чтобы были единичной длины
            // direction.print();

            direction.normalize();
            // this->_normalVertexMap[i][j].print();
            this->_normalVertexMap[i][j].normalize();

            // // direction.print();
            // this->_normalVertexMap[i][j].print();
            // вот она, интенсивность в вершине
            this->_intensityVertexMap[i][j] = this->_light.caclIntensityAtVertex(direction, this->_normalVertexMap[i][j]);

            // this->_map[i][j].print();
        }

    this->_printVertexIntensity();
}

void Landscape::_printVertexIntensity()
{
    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
        {
            std::cout << "[=] intensity = " << this->_intensityVertexMap[i][j] << std::endl;
        }
}

void Landscape::_calcFramebuffer(const Matrix<Point3D<double>> &screenMap)
{
    std::cout << "[B] _calcFramebuffer" << std::endl;

    const int rows = this->_map.size();
    const int cols = this->_map[0].size();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenMap[i][j], screenMap[i + 1][j], screenMap[i + 1][j + 1]);
            Plane plane2(screenMap[i][j], screenMap[i][j + 1], screenMap[i + 1][j + 1]);

            // определяем интенсивности вершин квадрата
            double I1 = this->_intensityVertexMap[i][j];
            double I2 = this->_intensityVertexMap[i + 1][j];
            double I3 = this->_intensityVertexMap[i + 1][j + 1];
            double I4 = this->_intensityVertexMap[i][j + 1];

            // определяем текущее состояние буфера кадра
            _zBuffer.calсFramebufferByPlane(plane1, I1, I2, I3);
            _zBuffer.calсFramebufferByPlane(plane2, I3, I4, I1);
        }
}

int Landscape::getWaterlevel() const
{
    return this->_waterlevel;
}

void Landscape::setWaterlevel(const int waterlevel)
{
    this->_waterlevel = waterlevel;
}

Point3D<double> Landscape::getCenterPoint() const
{
    return Point3D<double>(this->_centerPoint);
}

void Landscape::setCenterPoint(const Point3D<double> &centerPoint)
{
    this->_centerPoint = centerPoint;
}
