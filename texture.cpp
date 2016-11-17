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
        return qRgb(90,0,157);

    return this->pixel(u * this->width(), v * this->height());
}

