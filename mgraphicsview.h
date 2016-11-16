#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QtGui>
#include <QMainWindow>
#include <QGraphicsView>
#include "cammod.h"

class MGraphicsView : public QGraphicsView
{
     Q_OBJECT

public:
    MGraphicsView(QWidget *parent = 0);
    CamMod *cmod;

private:
    void mouseMoveEvent(QMouseEvent *pe);

};

#endif // MGRAPHICSVIEW_H
