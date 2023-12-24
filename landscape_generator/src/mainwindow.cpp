#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->landscapeGraphicsView->setScene(new QGraphicsScene);
    ui->landscapeGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->landscapeGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->waterlevelSlider, &QSlider::valueChanged, ui->waterLevelSpinbox, &QSpinBox::setValue);
    connect(ui->waterLevelSpinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->waterlevelSlider, &QSlider::setValue);

    connect(ui->seedSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeSeed()));
    connect(ui->octavesSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeOctaves()));
    connect(ui->frequencySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeFrequency()));
    connect(ui->lacunaritySpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeLacunarity()));
    connect(ui->amplitudeSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeAmplitude()));
    connect(ui->persistenceSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changePersistence()));

    connect(ui->lightXSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeLightPosition()));
    connect(ui->lightYSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeLightPosition()));
    connect(ui->lightZSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeLightPosition()));

    connect(ui->K_dSpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeLightK_d()));
    connect(ui->I_0SpinBox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeLightI_0()));

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

    connect(ui->scaleXSpinbox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeScaleParams()));
    connect(ui->scaleYSpinbox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeScaleParams()));
    connect(ui->scaleZSpinbox, SIGNAL(valueChanged(double)), this,
            SLOT(_changeScaleParams()));

    connect(ui->rotateXSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));
    connect(ui->rotateYSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));
    connect(ui->rotateZSpinbox, SIGNAL(valueChanged(int)), this,
            SLOT(_changeRotateParams()));

    this->_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_init()
{
    this->_paramNoise.setSeed(ui->seedSpinBox->value());
    this->_paramNoise.setOctaves(ui->octavesSpinBox->value());
    this->_paramNoise.setFrequency(ui->frequencySpinBox->value());
    this->_paramNoise.setLacunarity(ui->lacunaritySpinBox->value());
    this->_paramNoise.setAmplitude(ui->amplitudeSpinBox->value());
    this->_paramNoise.setPersistense(ui->persistenceSpinBox->value());

    this->_landscape.setMaxGenHeight(ui->maxHeightSpinBox->value());
    this->_landscape.resize(ui->widthSpinBox->value(), ui->lenghtSpinBox->value());

    this->_light.setPosition(QVector3D(ui->lightXSpinBox->value(),
                                       ui->lightYSpinBox->value(),
                                       ui->lightZSpinBox->value()));
    this->_light.setI_0(ui->I_0SpinBox->value());
    this->_light.setK_d(ui->K_dSpinBox->value());

    Move move = {ui->moveXSpinbox->value(), ui->moveYSpinbox->value(), ui->moveZSpinbox->value()};
    LandscapeManager::moveLandscape(this->_landscape, move);

    Rotate rotate = {ui->rotateXSpinbox->value(),
                     ui->rotateYSpinbox->value(),
                     ui->rotateZSpinbox->value()};
    QVector3D center(ui->rotateXcSpinBox->value(),
                     ui->rotateYcSpinBox->value(),
                     ui->rotateZcSpinBox->value());
    LandscapeManager::rotateLandscape(this->_landscape, rotate, center);

    Scale scale = {ui->scaleXSpinbox->value(),
                   ui->scaleYSpinbox->value(),
                   ui->scaleZSpinbox->value()};

    QVector3D center2(ui->scaleXcSpinBox->value(), ui->scaleYcSpinBox->value(), ui->scaleZcSpinBox->value());
    LandscapeManager::scaleLandscape(this->_landscape, scale, center2);

    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());

    ui->waterlevelSlider->setMaximum(this->_landscape.getMaxHeight());
    ui->waterLevelSpinbox->setMaximum(this->_landscape.getMaxHeight());
}

void MainWindow::_changeSeed()
{
    this->_paramNoise.setSeed(ui->seedSpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeOctaves()
{
    this->_paramNoise.setOctaves(ui->octavesSpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeFrequency()
{
    this->_paramNoise.setFrequency(ui->frequencySpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLacunarity()
{
    this->_paramNoise.setLacunarity(ui->lacunaritySpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeAmplitude()
{
    this->_paramNoise.setAmplitude(ui->amplitudeSpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changePersistence()
{
    this->_paramNoise.setPersistense(ui->persistenceSpinBox->value());
    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLightPosition()
{
    this->_light.setPosition(QVector3D(ui->lightXSpinBox->value(),
                                       ui->lightYSpinBox->value(),
                                       ui->lightZSpinBox->value()));
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLightK_d()
{
    this->_light.setK_d(ui->K_dSpinBox->value());
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLightI_0()
{
    this->_light.setI_0(ui->I_0SpinBox->value());
    LandscapeManager::calcIntensityForEachVertex(this->_landscape, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeLandscapeSize()
{
    int newWidth = ui->widthSpinBox->value();
    int newLenght = ui->lenghtSpinBox->value();

    this->_landscape.setMaxGenHeight(ui->maxHeightSpinBox->value());
    this->_landscape.resize(newWidth, newLenght);

    LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeMoveParams()
{
    Move move = {ui->moveXSpinbox->value(), ui->moveYSpinbox->value(), ui->moveZSpinbox->value()};

    LandscapeManager::moveLandscape(this->_landscape, move);

    LandscapeManager::updateLandscapeLight(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeRotateParams()
{
    Rotate rotate = {ui->rotateXSpinbox->value(), ui->rotateYSpinbox->value(), ui->rotateZSpinbox->value()};

    QVector3D centerRotate(ui->rotateXcSpinBox->value(), ui->rotateYcSpinBox->value(), ui->rotateZcSpinBox->value());

    LandscapeManager::rotateLandscape(this->_landscape, rotate, centerRotate);

    LandscapeManager::updateLandscapeLight(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::_changeScaleParams()
{
    Scale scale = {ui->scaleXSpinbox->value(), ui->scaleYSpinbox->value(), ui->scaleZSpinbox->value()};

    QVector3D centerScale(ui->scaleXcSpinBox->value(), ui->scaleYcSpinBox->value(), ui->scaleZcSpinBox->value());

    LandscapeManager::scaleLandscape(this->_landscape, scale, centerScale);

    LandscapeManager::updateLandscapeLight(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

void MainWindow::on_waterlevelSlider_valueChanged(int value)
{
    LandscapeManager::changeWaterlevel(this->_landscape, value);

    LandscapeManager::updateLandscapeLight(this->_landscape, this->_light);

    this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
}

double MainWindow::_getRenderLandscapeTimeBySize(const int size, const int nreps)
{
    std::cout << "[INFO] call _getRenderLandscapeTimeBySize ";
    std::cout << size << std::endl;

    this->_landscape.resize(size, size);

    this->_paramNoise.setSeed(ui->seedSpinBox->value());
    this->_paramNoise.setOctaves(ui->octavesSpinBox->value());
    this->_paramNoise.setFrequency(ui->frequencySpinBox->value());
    this->_paramNoise.setLacunarity(ui->lacunaritySpinBox->value());
    this->_paramNoise.setAmplitude(ui->amplitudeSpinBox->value());
    this->_paramNoise.setPersistense(ui->persistenceSpinBox->value());

    this->_landscape.setMaxGenHeight(ui->maxHeightSpinBox->value());

    this->_light.setPosition(QVector3D(ui->lightXSpinBox->value(),
                                       ui->lightYSpinBox->value(),
                                       ui->lightZSpinBox->value()));
    this->_light.setI_0(ui->I_0SpinBox->value());
    this->_light.setK_d(ui->K_dSpinBox->value());

    Move move = {ui->moveXSpinbox->value(), ui->moveYSpinbox->value(), ui->moveZSpinbox->value()};

    Rotate rotate = {ui->rotateXSpinbox->value(),
                     ui->rotateYSpinbox->value(),
                     ui->rotateZSpinbox->value()};
    QVector3D center(ui->rotateXcSpinBox->value(),
                     ui->rotateYcSpinBox->value(),
                     ui->rotateZcSpinBox->value());

    Scale scale = {ui->scaleXSpinbox->value(),
                   ui->scaleYSpinbox->value(),
                   ui->scaleZSpinbox->value()};

    QVector3D center2(ui->scaleXcSpinBox->value(),
                      ui->scaleYcSpinBox->value(),
                      ui->scaleZcSpinBox->value());

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nreps; ++i)
    {
        LandscapeManager::moveLandscape(this->_landscape, move);
        LandscapeManager::rotateLandscape(this->_landscape, rotate, center);
        LandscapeManager::scaleLandscape(this->_landscape, scale, center2);
        LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
        this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() / nreps;
}

void MainWindow::_measureRenderLandscapeTimeSize()
{
    std::cout << "[INFO] call _measureRenderLandscapeTime" << std::endl;

    vector<int> sizes = {10, 50, 100, 200, 300, 400, 500, 600, 700};

    std::ofstream dataResults("../landscape_generator/data/study1.txt");

    if (!dataResults.is_open())
    {
        std::cout << "[ERROR] Ошибка открытия файла" << std::endl;
        return;
    }

    for (int i = 0; i < sizes.size(); ++i)
    {
        double res = this->_getRenderLandscapeTimeBySize(sizes[i], 20);

        dataResults << sizes[i] << "   " << res << std::endl;
    }

    dataResults.close();

    std::cout << "[INFO] end _measureRenderLandscapeTime" << std::endl;
}

void MainWindow::_measureRenderLandscapeTimeOctaves()
{
    std::cout << "[INFO] call _measureRenderLandscapeTimeOctaves" << std::endl;

    vector<int> octaves = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::ofstream dataResults("../landscape_generator/data/study2.txt");

    if (!dataResults.is_open())
    {
        std::cout << "[ERROR] Ошибка открытия файла" << std::endl;
        return;
    }

    for (int i = 0; i < octaves.size(); ++i)
    {
        double res = this->_getZRenderLandscapeTimeByCntOctaves(octaves[i], 50);

        dataResults << octaves[i] << "   " << res << std::endl;
    }

    dataResults.close();

    std::cout << "[INFO] end _measureRenderLandscapeTimeOctaves" << std::endl;
}

double MainWindow::_getZRenderLandscapeTimeByCntOctaves(const int cntOctaves, const int nreps)
{
    std::cout << "[INFO] call _getZRenderLandscapeTimeByCntOctaves ";
    std::cout << cntOctaves << std::endl;

    this->_paramNoise.setSeed(ui->seedSpinBox->value());
    this->_paramNoise.setOctaves(cntOctaves);
    this->_paramNoise.setFrequency(ui->frequencySpinBox->value());
    this->_paramNoise.setLacunarity(ui->lacunaritySpinBox->value());
    this->_paramNoise.setAmplitude(ui->amplitudeSpinBox->value());
    this->_paramNoise.setPersistense(ui->persistenceSpinBox->value());

    this->_landscape.setMaxGenHeight(ui->maxHeightSpinBox->value());
    this->_landscape.resize(ui->widthSpinBox->value(), ui->lenghtSpinBox->value());

    this->_light.setPosition(QVector3D(ui->lightXSpinBox->value(),
                                       ui->lightYSpinBox->value(),
                                       ui->lightZSpinBox->value()));
    this->_light.setI_0(ui->I_0SpinBox->value());
    this->_light.setK_d(ui->K_dSpinBox->value());

    Move move = {ui->moveXSpinbox->value(), ui->moveYSpinbox->value(), ui->moveZSpinbox->value()};

    Rotate rotate = {ui->rotateXSpinbox->value(),
                     ui->rotateYSpinbox->value(),
                     ui->rotateZSpinbox->value()};
    QVector3D center(ui->rotateXcSpinBox->value(),
                     ui->rotateYcSpinBox->value(),
                     ui->rotateZcSpinBox->value());

    Scale scale = {ui->scaleXSpinbox->value(),
                   ui->scaleYSpinbox->value(),
                   ui->scaleZSpinbox->value()};

    QVector3D center2(ui->scaleXcSpinBox->value(),
                      ui->scaleYcSpinBox->value(),
                      ui->scaleZcSpinBox->value());

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nreps; ++i)
    {
        LandscapeManager::moveLandscape(this->_landscape, move);
        LandscapeManager::rotateLandscape(this->_landscape, rotate, center);
        LandscapeManager::scaleLandscape(this->_landscape, scale, center2);
        LandscapeManager::updateLandscape(this->_landscape, this->_paramNoise, this->_light);
        this->_renderer.renderLandscape(this->_landscape, ui->landscapeGraphicsView->scene());
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() / nreps;
}
