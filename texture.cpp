#include "texture.h"

Texture::Texture()
{

}

Texture::Texture(const char *name) : QImage(name)
{

}

QRgb Texture::texel(double u, double v)
{
    if (u <= 0 || u >= 1 || v <= 0 || v >= 1)
        return qRgb(0, 0, 0);

    return this->pixel(u * this->width(), v * this->height());
}

