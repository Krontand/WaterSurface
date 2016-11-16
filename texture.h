#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>

class Texture : public QImage
{
public:
    Texture();
    Texture(const char *name);

    QRgb texel(double u, double v);

};

#endif // TEXTURE_H
