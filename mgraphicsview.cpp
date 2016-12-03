#include "mgraphicsview.h"

MGraphicsView::MGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    viewport()->setMouseTracking(false);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *pe)
{
    cmod->mouse_move(pe);
    pe->ignore();
}

void MGraphicsView::mousePressEvent(QMouseEvent *pe)
{
    cmod->mouse_press(pe);
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *pe)
{
    cmod->mouse_release(pe);
    pe->ignore();
}

void MGraphicsView::wheelEvent(QWheelEvent *pe)
{
    cmod->mouse_wheel(pe);
}

void MGraphicsView::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == 32)
        cmod->reset_water();
}
