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
    connect(ui->maxHeightSpinBox, SIGNAL(valueChanged(int)), this,
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
    this->__changeLandscapeSize();

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

void MainWindow::__changeParamLight()
{
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
    std::cout << ui->landscapeGraphicsView->width() << std::endl;
    std::cout << ui->landscapeGraphicsView->height() << std::endl;

    this->__changeParamLight();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeMoveParams()
{
    Move move;
    move.dx = ui->moveXSpinbox->value();
    move.dy = ui->moveYSpinbox->value();
    move.dz = ui->moveZSpinbox->value();

    LandscapeManager::moveLandscape(this->_landscape, move);

    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
}

void MainWindow::_changeMoveParams()
{
    this->__changeMoveParams();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeRotateParams()
{
    Rotate rotate;
    rotate.xAngle = ui->rotateXSpinbox->value();
    rotate.yAngle = ui->rotateYSpinbox->value();
    rotate.zAngle = ui->rotateZSpinbox->value();

    LandscapeManager::rotateLandscape(this->_landscape, rotate);

    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
}

void MainWindow::_changeRotateParams()
{
    this->__changeRotateParams();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeLandscapeSize()
{
    int newWidth = ui->widthSpinBox->value();
    int newLenght = ui->lenghtSpinBox->value();

    this->_landscape.setMaxGenHeight(ui->maxHeightSpinBox->value());
    this->_landscape.resize(newWidth, newLenght);

    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
}

void MainWindow::_changeLandscapeSize()
{
    this->__changeLandscapeSize();
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::on_waterlevelSlider_valueChanged(int value)
{
    LandscapeManager::changeWaterlevel(this->_landscape, value);

    LandscapeManager::calcNormalForEachPlane(this->_landscape);
    LandscapeManager::calcNormalForEachVertex(this->_landscape);
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}
