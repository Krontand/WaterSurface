#include "mgraphicsview.h"

MGraphicsView::MGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    viewport()->setMouseTracking(false);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *pe)
{
    cmod->mouse_move(pe);
}

void MGraphicsView::mousePressEvent(QMouseEvent *pe)
{
    cmod->mouse_press(pe);
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *pe)
{
    cmod->mouse_release(pe);
}
