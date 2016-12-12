#include "imageitem.h"


#include <iostream>

ImageItem::ImageItem(int w, int h)
{
    image = QImage(w, h, QImage::Format_RGB32);
    QGraphicsPixmapItem(QPixmap::fromImage(image));
    this->pixels = image.scanLine(0);
    this->w = image.width();
    this->h = image.height();
}

ImageItem::ImageItem(const char *name)
{
    image = QImage(name);
    this->pixels = image.scanLine(0);
    this->w = image.width();
    this->h = image.height();
}

QRgb ImageItem::texel(double u, double v)
{
    if (u <= 0 || u >= 1 || v <= 0 || v >= 1)
        return qRgb(0,250,0);

    return this->image.pixel(u * this->w, v * this->h);
}

void ImageItem::advance(int phase)
{
    if (!phase)
    {
        buf = image;
        this->setPixmap(QPixmap::fromImage(image));
        memset(image.scanLine(0), 0, image.byteCount());
        pixels = this->image.scanLine(0);
    }

}

void ImageItem::fill(quint8 r, quint8 g, quint8 b)
{

    for (int i = 0; i < image.height(); i++)
    {
        quint8 *pix = image.scanLine(i);
        for (int j = 0; j < image.width(); j++)
        {
                pix[0] = b;
                pix[1] = g;
                pix[2] = r;
                pix++;
        }
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
    if ((x < 0) || (x > this->w) || (y < 0) || (y > this->h))
        return;
    quint8 *pix = (pixels + 4*this->w*y + 4*x);
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;
}

void ImageItem::set(int x, int y, Vector c)
{
    if ((x < 0) || (x > this->w) || (y < 0) || (y > this->h))
        return;
    quint8 *pix = (pixels + 4*this->w*y + 4*x);
    pix[0] = std::max(int(c[2]+.5), 0);
    pix[1] = std::max(int(c[1]+.5), 0);
    pix[2] = std::max(int(c[0]+.5), 0);

}


void ImageItem::set(Vector p, quint8 r, quint8 g, quint8 b)
{

    if ((p[0] < 0) || (p[0] > this->w) || (p[1] < 0) || (p[1] > this->h))
        return;
    quint8 *pix = image.scanLine((int)(p[1]+.5)) + ((int)(p[0]+.5))*4;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;
}
