#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "landscape.h"
#include <QMainWindow>

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

    void create_landscape();

private slots:

    void on_spinBox_waterlevel_valueChanged(int arg1);

    void on_printLandscape_clicked();

private:
    Ui::MainWindow *ui;
    Landscape _landscape;
};
#endif // MAINWINDOW_H
