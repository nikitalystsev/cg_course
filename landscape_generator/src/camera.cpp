#include "../inc/camera.h"

Camera::Camera(double aspect,
               Vector3D<double> position,
               Vector2D<double> rotation,
               double fovy) :
    _position(position),
    _currRotation(rotation),
    _view(4, vector<double>(4, 0)),
    _projection(4, vector<double>(4, 0)),
    _up(CAMERA_UP_VECTOR)
{
    this->rotate(this->_currRotation);
    this->calcProjectionMatrix(aspect, fovy);
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

    this->_target.setX(cos(Transform::toRadians(this->_currRotation.getX())) * cos(Transform::toRadians(this->_currRotation.getY())));

    this->_target.setY(sin(Transform::toRadians(this->_currRotation.getY())));

    this->_target.setZ(sin(Transform::toRadians(this->_currRotation.getX())) * cos(Transform::toRadians(this->_currRotation.getY())));

    this->_target.print();

    this->lookAt();
}
void Camera::move(const Vector3D<double> &movement)
{
    this->_position += movement;

    this->lookAt();
}

void Camera::lookAt()
{
    std::cout << "[B] lookAt" << std::endl;

    Vector3D<double> f = this->_target - this->_position;
    f.normalize();
    auto s = Vector3D<double>::vector_product(this->_up, f);
    s.normalize();
    auto u = Vector3D<double>::vector_product(f, s);

    vector<Vector3D<double>> arr;
    arr.push_back(s);
    arr.push_back(u);
    arr.push_back(f);

    this->_view[3][3] = 1;

    for (int i = 0; i < 3; ++i)
    {
        this->_view[0][i] = s[i];
        this->_view[1][i] = u[i];
        this->_view[2][i] = -(f[i]);
        this->_view[3][i] = -(Vector3D<double>::scalar_product(arr[i], this->_position));
    }
}

void Camera::calcProjectionMatrix(double aspect, double fovy)
{
    Matrix<double> resultProjection(4, vector<double>(4, 0));

    resultProjection[1][1] = 1 / tan(Transform::toRadians(fovy));
    resultProjection[0][0] = resultProjection[1][1] / aspect;
    resultProjection[2][2] = -(CAMERA_FAR + CAMERA_NEAR) / (CAMERA_FAR - CAMERA_NEAR);
    resultProjection[3][2] = -2 * CAMERA_NEAR * CAMERA_FAR / (CAMERA_FAR - CAMERA_NEAR);
    resultProjection[2][3] = -1;

    this->_projection = resultProjection;
}

Matrix<double> Camera::getView() const
{
    return this->_view;
}

Matrix<double> Camera::getProjection() const
{
    return this->_projection;
}
