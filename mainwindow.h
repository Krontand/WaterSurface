#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDesktopWidget>
#include "imageitem.h"
#include <QTimer>
#include <scene.h>
#include "cammod.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_powerSlider_sliderReleased();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    Scene *waterscene;
    CamMod *cmod;
};

#endif // MAINWINDOW_H
