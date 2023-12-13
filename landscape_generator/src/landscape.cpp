#include "../inc/landscape.h"
#include "../inc/perlinNoise.h"

Landscape::Landscape() :
    Landscape(default_width, default_lenght, default_waterlevel)
{
}

Landscape::Landscape(const int width, const int lenght, const int waterlevel) :
    _width(width), _lenght(lenght), _waterlevel(waterlevel),
    _rows(width + 1), _cols(lenght + 1),
    _map(width + 1, vector<Point3D<double>>(lenght + 1)),
    _withoutWaterlevelMap(width + 1, vector<double>(lenght + 1)),
    _normalMap(width + 1, vector<pair<Vector3D<double>, Vector3D<double>>>(lenght + 1)),
    _normalVertexMap(width + 1, vector<Vector3D<double>>(lenght + 1)),
    _intensityVertexMap(width + 1, vector<double>(lenght + 1)),
    _camera((double)1311 / 781, Vector3D<double>(300, -2000, 300), Vector2D<double>(0, 15), 30)
{
}

Landscape::~Landscape() {}

void Landscape::draw(QGraphicsScene *scene)
{
    std::cout << "[B] draw" << std::endl;

    Matrix<Point3D<double>> screenMap = this->_mapToScreen();

    this->_calcZBuffer(screenMap);
    this->_drawMap(scene);
}

template <typename T>
void resizeMatrix(Matrix<T> &matrix, int newWidth, int newLenght)
{
    matrix.resize(newWidth);

    for (int i = 0; i < newWidth; ++i)
        matrix[i].resize(newLenght);
}

void Landscape::resize(const int width, const int lenght)
{
    this->_width = width;
    this->_lenght = lenght;
    this->_rows = width + 1;
    this->_cols = lenght + 1;

    this->_map.clear();
    this->_withoutWaterlevelMap.clear();
    this->_normalMap.clear();
    this->_normalVertexMap.clear();
    this->_intensityVertexMap.clear();

    resizeMatrix<Point3D<double>>(this->_map, this->_rows, this->_cols);
    resizeMatrix<double>(this->_withoutWaterlevelMap, this->_rows, this->_cols);
    resizeMatrix<pair<Vector3D<double>, Vector3D<double>>>(this->_normalMap, this->_rows, this->_cols);
    resizeMatrix<Vector3D<double>>(this->_normalVertexMap, this->_rows, this->_cols);
    resizeMatrix<double>(this->_intensityVertexMap, this->_rows, this->_cols);
}

Matrix<Point3D<double>> Landscape::_mapToScreen()
{
    Matrix<Point3D<double>> tmp(this->_map);

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            Transform::pointToIsometric(tmp[i][j]);
        }

    this->_calcCenterPoint(tmp);

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            this->_movePointToCenter(tmp[i][j]);
        }

    return tmp;
}

// Matrix<Point3D<double>> Landscape::_mapToScreen()
//{
//     Matrix<Point3D<double>> tmp(this->_map);

//    Matrix<double> view = this->_camera.getView();
//    Matrix<double> projection = this->_camera.getProjection();

//    Matrix<double> vp = this->_matrixMul(view, projection);

//    for (int i = 0; i < this->_rows; ++i)
//        for (int j = 0; j < this->_cols; ++j)
//        {
//            Matrix<double> vertex(1, vector<double>(4, 0));
//            vertex[0][0] = tmp[i][j].getX();
//            vertex[0][1] = tmp[i][j].getY();
//            vertex[0][2] = tmp[i][j].getZ();

//            Matrix<double> res = this->_matrixMul(vertex, vp);

//            tmp[i][j].setX(res[0][0]);
//            tmp[i][j].setY(res[0][1]);
//            tmp[i][j].setZ(res[0][2]);

//            //            Transform::pointToIsometric(tmp[i][j]);
//        }

//    this->_calcCenterPoint(tmp);

//    for (int i = 0; i < this->_rows; ++i)
//        for (int j = 0; j < this->_cols; ++j)
//            this->_movePointToCenter(tmp[i][j]);

//    return tmp;
//}

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

            //  определяем текущее состояние z-буффера
            _zBuffer.calcZBufferByPlane(plane1, I1, I2, I3);
            _zBuffer.calcZBufferByPlane(plane2, I1, I4, I3);
        }
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

void Landscape::_calcCenterPoint(Matrix<Point3D<double>> &screenMap)
{
    int xMin = screenMap[0][0].getX(), yMin = screenMap[0][0].getY();
    int xMax = screenMap[0][0].getX(), yMax = screenMap[0][0].getY();

    for (int i = 0; i < this->_rows; ++i)
        for (int j = 0; j < this->_cols; ++j)
        {
            int currX = screenMap[i][j].getX(), currY = screenMap[i][j].getY();

            if (currX < xMin)
                xMin = currX;
            if (currY < yMin)
                yMin = currY;
            if (currX > xMax)
                xMax = currX;
            if (currY > yMax)
                yMax = currY;
        }

    this->_centerPoint.setX((xMin + xMax) / 2);
    this->_centerPoint.setY((yMin + yMax) / 2);
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
            _zBuffer.calсFramebufferByPlane(plane2, I1, I4, I3);
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

int Landscape::getWidth() const
{
    return this->_width;
}

void Landscape::setWidth(const int width)
{
    this->_width = width;
}

int Landscape::getLenght() const
{
    return this->_lenght;
}

void Landscape::setLenght(const int lenght)
{
    this->_width = lenght;
}

void Landscape::setMap(const Matrix<Point3D<double>> &map)
{
    this->_map = map;
}

Matrix<Point3D<double>> &Landscape::getMap()
{
    return this->_map;
}

void Landscape::setNormalMap(const Matrix<pair<Vector3D<double>, Vector3D<double>>> &map)
{
    this->_normalMap = map;
}

Matrix<pair<Vector3D<double>, Vector3D<double>>> &Landscape::getNormalMap()
{
    return this->_normalMap;
}

void Landscape::setNormalVertexMap(const Matrix<Vector3D<double>> &map)
{
    this->_normalVertexMap = map;
}

Matrix<Vector3D<double>> &Landscape::getNormalVertexMap()
{
    return this->_normalVertexMap;
}

void Landscape::setIntensityVertexMap(const Matrix<double> &map)
{
    this->_intensityVertexMap = map;
}

Matrix<double> &Landscape::getIntensityVertexMap()
{
    return this->_intensityVertexMap;
}
