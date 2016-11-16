#include "imageitem.h"


#include <iostream>

ImageItem::ImageItem(int w, int h)
{
    image = QImage(w, h, QImage::Format_RGB32);
    QGraphicsPixmapItem(QPixmap::fromImage(image));
}

void ImageItem::advance(int phase)
{
    if (!phase)
    {
        buf = image;
        this->setPixmap(QPixmap::fromImage(buf));
        image.byteCount();
        memset(image.scanLine(0), 0, image.byteCount());
    }
}

void ImageItem::setline(Vector p1, Vector p2, quint8 r, quint8 g, quint8 b)
{
    int xs = p1[0];
    int xe = p2[0];
    int ys = p1[1];
    int ye = p2[1];

    if (xs == xe && ys == ye)
    {
        set(xs, ys, r, g, b);
        return;
    }

    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    int ab;
    if (dx > dy)
        ab = 0;
    else
    {
        ab = 1;
        int t = dx;
        dx = dy;
        dy = t;
    }
    int dy2 = 2 * dy;
    int dx2 = 2 * dx;
    int e = dy2 - dx;
    for (int i = 1; i <= dx + 1; i++)
    {
        set(x, y, r, g, b);
        if (e > 0)
        {
            if (ab == 0)
                y += sy;
            else
                x += sx;
            e = e - dx2;
        }
        if (ab == 0)
            x += sx;
        else
            y += sy;
        e = e + dy2;
    }
}

void ImageItem::setline(int xs, int ys, int xe, int ye, quint8 r, quint8 g, quint8 b)
{
    if (xs == xe && ys == ye)
    {
        set(xs, ys, r, g, b);
        return;
    }

    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int sx = dx > 0 ? 1 : -1;
    int sy = dy > 0 ? 1 : -1;
    dx = abs(dx);
    dy = abs(dy);
    int ab;
    if (dx > dy)
        ab = 0;
    else
    {
        ab = 1;
        int t = dx;
        dx = dy;
        dy = t;
    }
    int dy2 = 2 * dy;
    int dx2 = 2 * dx;
    int e = dy2 - dx;
    for (int i = 1; i <= dx + 1; i++)
    {
        set(x, y, r, g, b);
        if (e > 0)
        {
            if (ab == 0)
                y += sy;
            else
                x += sx;
            e = e - dx2;
        }
        if (ab == 0)
            x += sx;
        else
            y += sy;
        e = e + dy2;
    }
}

void ImageItem::set(int x, int y, quint8 r, quint8 g, quint8 b)
{
    if ((x < 0) || (x > image.width()) || (y < 0) || (y > image.height()))
        return;
    quint8 *pix = image.scanLine(y) + x*4;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;
}

void ImageItem::set(int x, int y, Vector c)
{
    if ((x < 0) || (x > image.width()) || (y < 0) || (y > image.height()))
        return;
    quint8 *pix = image.scanLine(y) + x*4;
    pix[0] = std::max(int(c[2]+.5), 0);
    pix[1] = std::max(int(c[1]+.5), 0);
    pix[2] = std::max(int(c[0]+.5), 0);

}


void ImageItem::set(Vector p, quint8 r, quint8 g, quint8 b)
{   

    if ((p[0] < 0) || (p[0] > image.width()) || (p[1] < 0) || (p[1] > image.height()))
        return;
    quint8 *pix = image.scanLine((int)(p[1]+.5)) + ((int)(p[0]+.5))*4;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;
}
