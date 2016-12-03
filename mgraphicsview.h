#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QtGui>
#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>
#include "cammod.h"

class MGraphicsView : public QGraphicsView
{
     Q_OBJECT

public:
    MGraphicsView(QWidget *parent = 0);
    CamMod *cmod;

private:
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void keyPressEvent(QKeyEvent *ke);

};

#endif // MGRAPHICSVIEW_H
