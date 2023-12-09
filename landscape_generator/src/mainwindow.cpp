#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->view->setScene(new QGraphicsScene);
    ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->create_landscape();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_landscape()
{
    // std::cout << "[B] create_landscape" << std::endl;

    this->_landscape.generateHeightMap();
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
