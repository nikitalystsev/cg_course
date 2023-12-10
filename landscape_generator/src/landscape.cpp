#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(default_width, default_lenght, default_waterlevel, PerlinNoise(22, 8, 1, 4, 1, 0.25), Light(Point3D<int>(0, 0, 600), 1, 0.5))
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel, const PerlinNoise &paramNoise, const Light &light) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel), _paramNoise(paramNoise), _light(light),
    _rows(width + 1), _cols(lenght + 1),
    _map(width + 1, vector<Point3D<double>>(lenght + 1)),
    _withoutWaterlevelMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1))
{
}

Landscape::~Landscape() {}

void Landscape::generateHeightMap()
{
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            double nx = i / (double)this->_width - 0.5;
            double ny = j / (double)this->_lenght - 0.5;

            double height = this->_paramNoise.generateNoise(nx, ny);

            height *= 1000;

            if (height > this->_maxHeight)
                this->_maxHeight = height;

            if (height < _waterlevel)
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, _waterlevel);
            else
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, height);

            this->_withoutWaterlevelMap[i][j] = height;
        }
}

void Landscape::draw(QGraphicsScene *scene)
{
    std::cout << "[B] draw" << std::endl;

    Matrix<Point3D<double>> screenMap = this->_mapToScreen();

    //    this->_calcZBuffer(screenMap);
    this->_calcNormalForEachPlane();
    this->_calcNormalForEachVertex();
    this->_calcIntensityForEachVertex();
    this->_calcFramebuffer(screenMap);
    this->_drawMap(scene);
}

Matrix<Point3D<double>> Landscape::_mapToScreen()
{
    this->_calcCenterPoint();

    Matrix<Point3D<double>> tmp(this->_map);

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            this->_shiftPointToOrigin(tmp[i][j]);

            Transform::pointToIsometric(tmp[i][j]);

            this->_shiftPointBackToOrigin(tmp[i][j]);
            this->_movePointToCenter(tmp[i][j]);
        }

    this->_landscapeToCenterScene(tmp);

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
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

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows - 1; ++i)
        for (int j = 0; j < this->_cols - 1; ++j)
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
    Point3D<double> p1(this->_map[0][0]);
    Point3D<double> p2(this->_map[this->_rows - 1][this->_cols - 1]);

    this->_centerPoint.setX((p1.getX() + p2.getX()) / 2);
    this->_centerPoint.setY((p1.getY() + p2.getY()) / 2);

    double zMin = p1.getZ(), zMax = p1.getZ();

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
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

void Landscape::_landscapeToCenterScene(Matrix<Point3D<double>> &screenMap)
{
    Point2D<int> pMin(screenMap[0][0].getX(), screenMap[0][0].getY());
    Point2D<int> pMax(screenMap[0][0].getX(), screenMap[0][0].getY());

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            if (screenMap[i][j].getX() < pMin.getX() && screenMap[i][j].getY() < pMin.getY())
                pMin.set(screenMap[i][j].getX(), screenMap[i][j].getY());
            if (screenMap[i][j].getX() > pMax.getX() && screenMap[i][j].getY() > pMax.getY())
                pMax.set(screenMap[i][j].getX(), screenMap[i][j].getY());
        }

    this->_centerPoint.setX((pMin.getX() + pMax.getX()) / 2);
    this->_centerPoint.setY((pMin.getY() + pMax.getY()) / 2);
}

void Landscape::_calcNormalForEachPlane()
{
    std::cout << "[B] _calcNormalForEachPlane" << std::endl;

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows - 1; ++i)
        for (int j = 0; j < this->_cols - 1; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(this->_map[i][j], this->_map[i + 1][j], this->_map[i + 1][j + 1]);
            Plane plane2(this->_map[i][j], this->_map[i + 1][j + 1], this->_map[i][j + 1]);

            // получаем вектора внешних нормалей к граням
            Vector3D<double> normalPlane1(plane1.getA(), plane1.getB(), plane1.getC());
            Vector3D<double> normalPlane2(plane2.getA(), plane2.getB(), plane2.getC());

            normalPlane1.normalize();
            normalPlane2.normalize();

            pair<Vector3D<double>, Vector3D<double>> normals(normalPlane1, normalPlane2);

            this->_normalMap[i][j] = normals;
        }

    // this->_printPlaneNormals();
}

void Landscape::_printPlaneNormals()
{
    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows - 1; ++i)
        for (int j = 0; j < this->_cols - 1; ++j)
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

    // идем по всем вершинам ландшафтной сетки
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            Vector3D<double> avgNormal;
            // левая верхняя и правая нижняя вершины
            if ((i == 0 && j == 0) || (i == this->_rows - 1 && j == this->_cols - 1))
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
            }
            // правая верхняя и левая нижняя вершина
            else if ((i == 0 && j == this->_cols - 1) || (i == this->_rows - 1 && j == 0))
            {
                if (i == 0 && j == this->_cols - 1) // правая верхняя
                    avgNormal = this->_normalMap[i][j - 1].first;
                else // левая верхняя
                    avgNormal = this->_normalMap[i - 1][j].second;
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
            }
            this->_normalVertexMap[i][j] = avgNormal;
        }
    // this->_printVertexNormals();
}

void Landscape::_printVertexNormals()
{
    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            this->_normalVertexMap[i][j].print();
        }
}

void Landscape::_calcIntensityForEachVertex()
{
    std::cout << "[B] _caclIntensityForEachVertex" << std::endl;

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            // получили вектор направления света
            Vector3D<double> direction = this->_light.caclDirectionVector(this->_map[i][j]);
            // // нормализуем вектора, чтобы были единичной длины
            direction.normalize();
            this->_normalVertexMap[i][j].normalize();

            // вот она, интенсивность в вершине
            this->_intensityVertexMap[i][j] = this->_light.caclIntensityAtVertex(direction, this->_normalVertexMap[i][j]);
        }

    // this->_printVertexIntensity();
}

void Landscape::_printVertexIntensity()
{
    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows; ++i)
    {
        for (int j = 0; j < this->_cols; ++j)
        {
            if (this->_intensityVertexMap[i][j] != 0)
                std::cout << " ";
            else
                std::cout << this->_intensityVertexMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Landscape::_calcFramebuffer(const Matrix<Point3D<double>> &screenMap)
{
    std::cout << "[B] _calcFramebuffer" << std::endl;

    this->_zBuffer.clean();

    // идем по всем квадратам ландшафной сетки
    for (int i = 0; i < this->_rows - 1; ++i)
        for (int j = 0; j < this->_cols - 1; ++j)
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

void Landscape::updateWaterlevel(const int waterlevel)
{
    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
            if (this->_withoutWaterlevelMap[i][j] < waterlevel)
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, waterlevel);
            else
                this->_map[i][j].set(i * this->poly_size, j * this->poly_size, this->_withoutWaterlevelMap[i][j]);

    this->_waterlevel = waterlevel;
}

int Landscape::getMaxHeight() const
{
    return this->_maxHeight;
}

void Landscape::setMaxHeight(const double maxHeight)
{
    this->_maxHeight = maxHeight;
}

Point3D<double> Landscape::getCenterPoint() const
{
    return Point3D<double>(this->_centerPoint);
}

void Landscape::setCenterPoint(const Point3D<double> &centerPoint)
{
    this->_centerPoint = centerPoint;
}

PerlinNoise Landscape::getParamNoise() const
{
    return PerlinNoise(this->_paramNoise);
}

void Landscape::setParamNoise(const PerlinNoise &paramNoise)
{
    this->_paramNoise = paramNoise;
}

Light Landscape::getLight() const
{
    return Light(this->_light);
}

void Landscape::setLight(const Light &light)
{
    this->_light = light;
}
