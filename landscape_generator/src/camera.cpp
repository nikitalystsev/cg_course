#include "../inc/camera.h"

Camera::Camera(double aspect,
               Vector3D<double> position,
               Vector2D<double> rotation,
               double fovy) :
    _position(position),
    _currRotation(rotation),
    _up(CAMERA_UP_VECTOR),
    _view(4, vector<double>(4, 0)),
    _projection(4, vector<double>(4, 0))
{
    // сразу просчитываем вектор направления камеры
    this->_calcTarget();
    // сразу просчитываем матрицу вида
    this->lookAt();
    // исходя из соотношения сторон и вертикального угла обзора
    // просчитываем матрицу проекции (пирамиду видимости)
    this->calcProjectionMatrix(aspect, fovy);
}
void Camera::_calcTarget()
{
    double rotationX = this->_currRotation.getX();
    double rotationY = this->_currRotation.getY();

    if (rotationY >= 90)
    {
        rotationY = 90;
    }
    if (rotationY <= -90)
    {
        rotationY = -90;
    }
    if (rotationX >= 180)
    {
        rotationX = -180;
    }
    if (rotationX <= -180)
    {
        rotationX = 180;
    }

    double targetX = cos(Transform::toRadians(rotationX)) * cos(Transform::toRadians(rotationY));
    double targetY = cos(Transform::toRadians(rotationY));
    double targetZ = sin(Transform::toRadians(rotationX)) * cos(Transform::toRadians(rotationY));

    this->_target.set(targetX, targetY, targetZ);
    this->_currRotation.set(rotationX, rotationY);
}

void Camera::rotate(const Vector2D<double> &rotation)
{
    this->_currRotation += rotation;

    if (this->_currRotation.getY() >= 90)
        this->_currRotation.setY(90);
    if (this->_currRotation.getY() <= -90)
        this->_currRotation.setY(-90);
    if (this->_currRotation.getX() >= 180)
        this->_currRotation.setX(-180);
    if (this->_currRotation.getX() <= -180)
        this->_currRotation.setX(180);

    this->_target.setX(cos(Transform::toRadians(this->_currRotation.getX())) *
                       cos(Transform::toRadians(this->_currRotation.getY())));
    this->_target.setY(sin(Transform::toRadians(this->_currRotation.getY())));
    this->_target.setZ(sin(Transform::toRadians(this->_currRotation.getX())) *
                       cos(Transform::toRadians(this->_currRotation.getY())));

    this->lookAt();
}
void Camera::move(const Vector3D<double> &movement)
{
    this->_position += movement;

    this->lookAt();
}

Matrix<double> matrixMul(
    const Matrix<double> &mtr1,
    const Matrix<double> &mtr2)
{

    Matrix<double> mtrRes(mtr1.size(), vector<double>(mtr2[0].size(), 0));

    for (int i = 0; i < mtr1.size(); ++i)
        for (int j = 0; j < mtr2[0].size(); ++j)
            for (int k = 0; k < mtr1[0].size(); ++k)
                mtrRes[i][j] += mtr1[i][k] * mtr2[k][j];

    return mtrRes;
}

void Camera::lookAt()
{
    // target должна быть посчитана ранее
    // вектор, обратный вектору направления взгляда камеры
    Vector3D<double> _direction = this->_position - this->_target;
    _direction.normalize();

    // вектор вправо (представляет положительное направление оси Х камеры)
    auto right = Vector3D<double>::vector_product(this->_up, _direction);
    right.normalize();

    // вектор вверх (представляет положительное направление оси Y камеры)
    auto up = Vector3D<double>::vector_product(_direction, right);

    // левая матрица перемножения для матрицы вида
    Matrix<double> tmp1(4, vector<double>(4, 0));

    tmp1[3][3] = 1;

    // идем по строкам матрицы tmp1
    for (int i = 0; i < 3; ++i)
        if (i == 0)
        {
            tmp1[i][0] = right.getX();
            tmp1[i][1] = right.getY();
            tmp1[i][2] = right.getZ();
        }
        else if (i == 1)
        {
            tmp1[i][0] = up.getX();
            tmp1[i][1] = up.getY();
            tmp1[i][2] = up.getZ();
        }
        else
        {
            tmp1[i][0] = _direction.getX();
            tmp1[i][1] = _direction.getY();
            tmp1[i][2] = _direction.getZ();
        }

    // левая матрица перемножения для матрицы вида
    Matrix<double> tmp2(4, vector<double>(4, 0));

    // делаем вторую матрицу единичной
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            tmp2[i][j] = (i == j);

    // это указываются как бы инвертированные значения переноса
    tmp2[0][3] = -this->_position.getX();
    tmp2[1][3] = -this->_position.getY();
    tmp2[2][3] = -this->_position.getZ();

    // получили матрицу вида
    this->_view = matrixMul(tmp1, tmp2);
}

void Camera::calcProjectionMatrix(double aspect, double fovy)
{
    this->_projection[1][1] = 1 / tan(Transform::toRadians(fovy));
    this->_projection[0][0] = this->_projection[1][1] / aspect;
    this->_projection[2][2] = -(CAMERA_FAR + CAMERA_NEAR) / (CAMERA_FAR - CAMERA_NEAR);
    this->_projection[3][2] = -2 * CAMERA_NEAR * CAMERA_FAR / (CAMERA_FAR - CAMERA_NEAR);
    this->_projection[2][3] = -1;
}

Matrix<double> Camera::getView() const
{
    return this->_view;
}

Matrix<double> Camera::getProjection() const
{
    return this->_projection;
}
