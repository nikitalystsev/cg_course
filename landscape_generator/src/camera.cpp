#include "../inc/camera.h"

Matrix<double> Camera::lookAt()
{
    Vector3D<double> f = this->_target - this->_position;
    f.normalize();

    auto s = Vector3D<double>::vector_product(this->_up, f);
    s.normalize();

    auto u = Vector3D<double>::vector_product(f, s);

    vector<Vector3D<double>> arr;
    arr.push_back(s);
    arr.push_back(u);
    arr.push_back(f);

    Matrix<double> resultView(4, vector<double>(4, 0));

    resultView[3][3] = 1;

    for (int i = 0; i < 3; ++i)
    {
        resultView[0][i] = s[i];
        resultView[1][i] = u[i];
        resultView[2][i] = -(f[i]);
        resultView[3][i] = -(Vector3D<double>::scalar_product(arr[i], this->_position));
    }

    this->_view = resultView;

    return resultView;
}

Matrix<double> Camera::calcProjectionMatrix()
{
    Matrix<double> resultProjection(4, vector<double>(4, 0));

    resultProjection[1][1] = 1 / tan(Transform::toRadians(CAMERA_FOVy));
    resultProjection[0][0] = resultProjection[1][1] / (double)(1311 / 781);
    resultProjection[2][2] = -(CAMERA_FAR + CAMERA_NEAR) / (CAMERA_FAR - CAMERA_NEAR);
    resultProjection[3][2] = -2 * CAMERA_NEAR * CAMERA_FAR / (CAMERA_FAR - CAMERA_NEAR);
    resultProjection[2][3] = -1;

    this->_projection = resultProjection;

    return resultProjection;
}
