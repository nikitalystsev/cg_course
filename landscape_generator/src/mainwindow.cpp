#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->landscapeGraphicsView->setScene(new QGraphicsScene);
    ui->landscapeGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->landscapeGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->seedSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeParamNoise()));
    connect(ui->octavesSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeParamNoise()));
    connect(ui->frequencySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamNoise()));
    connect(ui->lacunaritySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamNoise()));
    connect(ui->amplitudeSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamNoise()));
    connect(ui->persistenceSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamNoise()));

    connect(ui->lightXSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeParamLight()));
    connect(ui->lightYSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeParamLight()));
    connect(ui->lightZSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeParamLight()));

    connect(ui->K_dSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamLight()));
    connect(ui->I_0SpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeParamLight()));

    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeLandscapeSize()));
    connect(ui->lenghtSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeLandscapeSize()));

    connect(ui->moveXSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeMoveParams()));
    connect(ui->moveYSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeMoveParams()));
    connect(ui->moveZSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeMoveParams()));

    connect(ui->rotateXSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));
    connect(ui->rotateYSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));
    connect(ui->rotateZSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));

    this->__changeParamNoise();
    this->__changeParamLight();
    this->__changeMoveParams();
    this->__changeRotateParams();

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::__changeParamNoise()
{
    int seed = ui->seedSpinBox->value();
    int octaves = ui->octavesSpinBox->value();
    double frequency = ui->frequencySpinBox->value();
    double lacunarity = ui->lacunaritySpinBox->value();
    double amplitude = ui->amplitudeSpinBox->value();
    double persistence = ui->persistenceSpinBox->value();

    this->_paramNoise = PerlinNoise(seed, octaves, frequency, lacunarity, amplitude, persistence);

    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
}

void MainWindow::_changeParamNoise()
{
    this->__changeParamNoise();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeMoveParams()
{
    move_t move;
    move.dx = ui->moveXSpinbox->value();
    move.dy = ui->moveYSpinbox->value();
    move.dz = ui->moveZSpinbox->value();

    LandscapeManager::moveLandscape(this->_landscape, move);

    //    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    //    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
}

void MainWindow::_changeMoveParams()
{
    move_t move;
    move.dx = ui->moveXSpinbox->value();
    move.dy = ui->moveYSpinbox->value();
    move.dz = ui->moveZSpinbox->value();

    LandscapeManager::moveLandscape(this->_landscape, move);

    //    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    //    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);

    // изображение рендерится сразу, центральная точка пиксельного изображения остается такой же
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeRotateParams()
{
    rotate_t rotate;
    rotate.xAngle = ui->rotateXSpinbox->value();
    rotate.yAngle = ui->rotateYSpinbox->value();
    rotate.zAngle = ui->rotateZSpinbox->value();

    LandscapeManager::rotateLandscape(this->_landscape, rotate);

    //    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    //    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);

    // пересчитываем центральную точку
    this->_renderer.calcCenterPoint(this->_landscape.getHeightMap());

    this->_renderer.moveLandscapeToCenter(this->_landscape);
}

void MainWindow::_changeRotateParams()
{
    rotate_t rotate;
    rotate.xAngle = ui->rotateXSpinbox->value();
    rotate.yAngle = ui->rotateYSpinbox->value();
    rotate.zAngle = ui->rotateZSpinbox->value();

    LandscapeManager::rotateLandscape(this->_landscape, rotate);

    //    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    //    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);

    // пересчитываем центральную точку
    this->_renderer.calcCenterPoint(this->_landscape.getHeightMap());

    this->_renderer.moveLandscapeToCenter(this->_landscape);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeParamLight()
{

    std::cout << ui->landscapeGraphicsView->width() << std::endl;
    std::cout << ui->landscapeGraphicsView->height() << std::endl;

    QVector3D lightPosition(ui->lightXSpinBox->value(),
                            ui->lightYSpinBox->value(),
                            ui->lightZSpinBox->value());

    double K_d = ui->K_dSpinBox->value();
    double I_0 = ui->I_0SpinBox->value();

    this->_light = Light(lightPosition, K_d, I_0);

    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
}

void MainWindow::_changeParamLight()
{
    this->__changeParamLight();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLandscapeSize()
{
    int newWidth = ui->widthSpinBox->value();
    int newLenght = ui->lenghtSpinBox->value();

    this->_landscape.resize(newWidth, newLenght);

    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::on_waterlevelSlider_valueChanged(int value)
{
    this->_landscape.updateWaterlevel(value);

    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}
