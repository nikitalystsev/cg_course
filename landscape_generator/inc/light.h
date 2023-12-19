#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <algorithm>

// класс точечного источника освещения
class Light
{
private:
    // положение точечного источника в пространстве
    QVector3D _position;
    // интенсивность точечного источника
    double _I_0;
    // коэффициент диффузного отражения (0 <= K_d <= 1)
    double _K_d;

public:
    Light();
    Light(const QVector3D &position, double I_0, double K_d);
    explicit Light(const Light &other);
    Light(Light &&other) noexcept;

    Light &operator=(const Light &other);
    Light &operator=(Light &&other) noexcept;

    QVector3D getPosition() const { return this->_position; };
    void setPosition(const QVector3D &position) { this->_position = position; };

    double getI_0() const { return this->_I_0; }
    double getK_d() const { return this->_K_d; }
    void setI_0(const double &I_0) { this->_I_0 = I_0; }
    void setK_d(const double &K_d) { this->_K_d = K_d; }
};

#endif // LIGHT_H
