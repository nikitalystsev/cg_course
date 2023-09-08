#include "../inc/mainwindow.h"
#include "../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->view->setScene(new QGraphicsScene);

    this->create_landscape();
    //        std::cout << ui->view->width() << std::endl;
    //        std::cout << ui->view->height() << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_landscape()
{
    //    std::cout << "call create_landscape" << std::endl;

    this->_landscape.generateHeightMap();
}

void MainWindow::on_printLandscape_clicked()
{
    //    std::cout << "call print_landscape" << std::endl;

    this->_landscape.draw(ui->view->scene());
}

void MainWindow::on_spinBox_waterlevel_valueChanged(int arg1)
{
    std::cout << "arg1 = " << arg1 << std::endl;

    //    this->_landscape.waterlevel(arg1);

    //    this->_landscape.generate();
    //    this->_landscape.points_to_screen();

    //    this->_landscape.draw(ui->view->scene());
}
