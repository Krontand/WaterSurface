#include "texture.h"

Texture::Texture(int w, int h) : QImage(w, h, QImage::Format_RGB32)
{

}

Texture::Texture(const char *name) : QImage(name)
{

}

QRgb Texture::texel(double u, double v)
{
    if (u <= 0 || u >= 1 || v <= 0 || v >= 1)
        return qRgb(0,250,0);

    return this->pixel(u * this->width(), v * this->height());
}

void Texture::set(int x, int y, quint8 r, quint8 g, quint8 b)
{
    if ((x < 0) || (x > this->width()) || (y < 0) || (y > this->height()))
        return;
    quint8 *pix = this->scanLine(y) + x*4;
    pix[0] = b;
    pix[1] = g;
    pix[2] = r;
}

