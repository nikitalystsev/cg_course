#include "../inc/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Генератор трехмерного ландшафта");
    w.show();

    return a.exec();
}
