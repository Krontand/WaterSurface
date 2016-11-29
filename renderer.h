#ifndef RENDERER_H
#define RENDERER_H

#include "geometry/geometry.h"
#include "imageitem.h"
#include "model/watermodel.h"
#include "model/poolmodel.h"
#include "model/skybox.h"
#include "camera.h"


class Renderer
{
public:
    Renderer(int w, int h);
    ~Renderer();

    void render(ImageItem *img, WaterModel *water, PoolModel *pool, Skybox *sky, Camera *cam, double part, char flags);

private:
    /*
     * Вычислить матрицу перехода к СК камеры
     */
    void setviewmatr(Camera *cam);

    /*
     * Вычислить матрицу центрального проецирования
     */
    void setprojmatr(Camera *cam);

    /*
     * Вычислить матрицу перехода к экранным координатам
     */
    void setViewPort(int x, int y, int w, int h, double part);

    /*
     * Закрасить полигон (с ненулевой прозрачностью)
     * p - вершины
     * i - интенсивности
     * c_ - показатель прозрачности полигона
     */
    void triangle(const struct PolyVecs &p, struct PolyI i, double c_);

    void triangle(const struct PolyVecs &p, struct PolyI i, TrPolygon uv, ImageItem *tex, double c_);

    /*
     * Закрасить полигон текстурой
     * p - вершины
     * i_ - интенсивности
     * uv - UV-координаты вершин полигона
     * tex - текстура
     * usez - если false, то рисуется, не затрагивая Z-буфер (для скайбокса)
     */
    void triangle(const struct PolyVecs &p, Vector i_, TrPolygon uv, ImageItem *tex, bool usez = true);

    Matrix projMatr;            // Матрица перспективного искажения
    Matrix viewMatr;            // Матрица перехода к СК камеры
    Matrix viewPort;            // Матрица перехода к экранным координатам
    int *zbuf;                  // Z-буфер
    Matrix ibuf;                // Буфер интенсивностей непрозрачных полигонов
    Matrix ibuf_transparent;    // Буфер интенсивностей прозрачных полигонов
    double *c_tr;               // Буфер взвешенных коэффициентов интенсивностей прозрачных полигонов
    int w;
    int h;

    ImageItem *img;             // Изображение
};

#endif // RENDERER_H
