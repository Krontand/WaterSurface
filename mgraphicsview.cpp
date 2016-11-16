#include "mgraphicsview.h"

MGraphicsView::MGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    viewport()->setMouseTracking(false);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *pe)
{
    cmod->mouseEvent(pe);
}
