#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
  //  QTimer *timer1 = new QTimer(this);

    waterscene = new Scene(0, 0, ui->graphicsView->width() - 5, ui->graphicsView->height() - 5);

    waterscene->set_changed();

    ui->graphicsView->setScene(waterscene->scene);
    cmod = new CamMod(this);
    ui->graphicsView->cmod = this->cmod;
    cmod->scene = waterscene;

    connect(timer, SIGNAL(timeout()), waterscene->scene, SLOT(advance()));
    connect(timer, SIGNAL(timeout()), cmod, SLOT(updatescene()));
 //   connect(timer1, SIGNAL(timeout()), cmod, SLOT(rand_disturb()));
    timer->start(1000/30);
//    timer1->start(1000*7);
}

MainWindow::~MainWindow()
{
    delete ui;
}
