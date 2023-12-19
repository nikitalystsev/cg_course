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
    void _init();

private slots:
    void _changeSeed();
    void _changeOctaves();
    void _changeFrequency();
    void _changeLacunarity();
    void _changeAmplitude();
    void _changePersistence();

    void _changeLightPosition();
    void _changeLightK_d();
    void _changeLightI_0();

    void _changeLandscapeSize();

    void _changeMoveParams();
    void _changeRotateParams();
    void _changeScaleParams();

    void on_waterlevelSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    PerlinNoise _paramNoise;
    Light _light;
    Landscape _landscape;
    Renderer _renderer;
};
#endif // MAINWINDOW_H
