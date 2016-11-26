#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>

class Texture : public QImage
{
public:
    Texture(int w, int h);
    Texture(const char *name);

    /*
     * Получить тексель с координатами (u, v)
     */
    QRgb texel(double u, double v);

    /*
     * Установить цвет пикселя
     */
    void set(int x, int y, quint8 r, quint8 g, quint8 b);
};

#endif // TEXTURE_H
