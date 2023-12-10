#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->view->setScene(new QGraphicsScene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->seedSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(create_landscape()));
    connect(ui->octavesSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(create_landscape()));
    connect(ui->frequencySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(create_landscape()));
    connect(ui->lacunaritySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(create_landscape()));
    connect(ui->amplitudeSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(create_landscape()));
    connect(ui->persistenceSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(create_landscape()));

    this->create_landscape();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_landscape()
{
    std::cout << "[B] create_landscape" << std::endl;

    int seed = ui->seedSpinBox->value();
    int octaves = ui->octavesSpinBox->value();
    double frequency = ui->frequencySpinBox->value();
    double lacunarity = ui->lacunaritySpinBox->value();
    double amplitude = ui->amplitudeSpinBox->value();
    double persistence = ui->persistenceSpinBox->value();

    PerlinNoise paramNoise(seed, octaves, frequency, lacunarity, amplitude, persistence);

    this->_landscape.setParamNoise(paramNoise);

    this->_landscape.generateHeightMap();
    this->_landscape.draw(ui->view->scene());
}

void MainWindow::on_printLandscape_clicked()
{
    //    std::cout << "call print_landscape" << std::endl;

    this->_landscape.draw(ui->view->scene());
}

void MainWindow::on_changeWaterlevel_valueChanged(int value)
{
    this->_landscape.setWaterlevel(value);

    this->_landscape.generateHeightMap();
    this->_landscape.draw(ui->view->scene());
}
