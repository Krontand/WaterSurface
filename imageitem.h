#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsPixmapItem>
#include <QImage>
#include "geometry/geometry.h"

class ImageItem : public QGraphicsPixmapItem
{
public:
    ImageItem(int w, int h);

    ImageItem(const char *name);

    /*
     * Получить тексель с координатами (u, v)
     */
    QRgb texel(double u, double v);


    /*
     * Вывод изображения на экран
     */
    virtual void advance(int phase);

    /*
     * Нарисовать линию от p1 до p2 цвета rgb
     */
    void setline(Vector p1, Vector p2, quint8 r, quint8 g, quint8 b);

    /*
     * Нарисовать линию от (xs, ys) до (xe, ye) цвета rgb
     */
    void setline(int xs, int ys, int xe, int ye, quint8 r, quint8 g, quint8 b);

    /*
     * Установить цвет пикселя
     */
    void set(Vector p, quint8 r, quint8 g, quint8 b);
    void set(int x, int y, quint8 r, quint8 g, quint8 b);
    void set(int x, int y, Vector c);

    /*
     * Заполнить изображение цветом
     */
    void fill(quint8 r, quint8 g, quint8 b);

    QImage image;
    QImage buf;
};

#endif // IMAGEITEM_H
