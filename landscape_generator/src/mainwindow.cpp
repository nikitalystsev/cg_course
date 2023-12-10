#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->landscapeGraphicsView->setScene(new QGraphicsScene);
    ui->landscapeGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->landscapeGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    std::cout << "[=] ui->landscapeGraphicsView->width() = " << ui->landscapeGraphicsView->width()
              << std::endl;

    std::cout << "[=] ui->landscapeGraphicsView->height() = " << ui->landscapeGraphicsView->height()
              << std::endl;

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

    this->__changeParamNoise();
    this->__changeParamLight();
    this->_landscape.draw(ui->landscapeGraphicsView->scene());
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

    PerlinNoise paramNoise(seed, octaves, frequency, lacunarity, amplitude, persistence);

    this->_landscape.setParamNoise(paramNoise);
    this->_landscape.generateHeightMap();
}

void MainWindow::_changeParamNoise()
{
    this->__changeParamNoise();
    this->_landscape.draw(ui->landscapeGraphicsView->scene());
}

void MainWindow::__changeParamLight()
{
    Point3D<int> lightPosition(ui->lightXSpinBox->value(),
                               ui->lightYSpinBox->value(),
                               ui->lightZSpinBox->value());

    double K_d = ui->K_dSpinBox->value();
    double I_0 = ui->I_0SpinBox->value();

    Light light(lightPosition, K_d, I_0);
    this->_landscape.setLight(light);
}

void MainWindow::_changeParamLight()
{
    this->__changeParamLight();
    this->_landscape.draw(ui->landscapeGraphicsView->scene());
}

void MainWindow::on_waterlevelSlider_valueChanged(int value)
{
    this->_landscape.updateWaterlevel(value);
    this->_landscape.draw(ui->landscapeGraphicsView->scene());
}
