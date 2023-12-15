#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "landscape.h"
#include "landscapeManager.h"
#include "renderer.h"
#include <QMainWindow>
#include <QVector3D>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void __changeParamNoise();
    void __changeParamLight();
    void __changeMoveParams();
    void __changeScaleParams();
    void __changeRotateParams();
    void __changeLandscapeSize();
private slots:
    void _changeParamNoise();

    void _changeParamLight();

    void _changeLandscapeSize();

    void _changeMoveParams();

    void _changeScaleParams();

    void _changeRotateParams();

    void on_waterlevelSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    PerlinNoise _paramNoise;
    Light _light;
    Landscape _landscape;
    Renderer _renderer;
};
#endif // MAINWINDOW_H
