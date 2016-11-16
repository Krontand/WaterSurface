#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include "geometry/geometry.h"

class ImageItem : public QGraphicsPixmapItem
{
public:
    ImageItem(int w, int h);
    virtual void advance(int phase);
    void clearimg();
    void setline(Vector p1, Vector p2, quint8 r, quint8 g, quint8 b);
    void setline(int xs, int ys, int xe, int ye, quint8 r, quint8 g, quint8 b);
    void set(Vector p, quint8 r, quint8 g, quint8 b);
    void set(int x, int y, quint8 r, quint8 g, quint8 b);
    void set(int x, int y, Vector c);

private:
    QImage image;
    QImage buf;
};

#endif // IMAGEITEM_H
