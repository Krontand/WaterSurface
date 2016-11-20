#ifndef Scene_H
#define Scene_H

#include "imageitem.h"
#include "model/watermodel.h"
#include "camera.h"
#include "geometry/geometry.h"
#include "math.h"
#include "model/poolmodel.h"
#include "texture.h"
#include "model/skybox.h"
#include <QRgb>
#include <QGraphicsScene>

#define SPEC_POWER 20.0
#define RCOLOR 0
#define GCOLOR 149
#define BCOLOR 182

class Scene
{
public:
    Scene(int x, int y, int w, int h);
    ~Scene();

    /*
     * Вычислить матрицу перехода к СК камеры
     */
    void setviewmatr();

    /*
     * Вычислить матрицу центрального проецирования
     */
    void setprojmatr();

    /*
     * Вычислить матрицу перехода к экранным координатам
     */
    void setViewPort(int x, int y, int w, int h);

    /*
     * Нарисовать сцену
     */
    void drawimage();

    /*
     * Вычислить новые высоты поверхности воды (вызывает метод модели)
     */
    void updheights();

    /*
     * Создать возмущение в случайном месте поверхности воды
     */
    void rand_disturb();    

    /*
     * Вычислить нормали поверхности воды
     */
    void calc_normals();

    /*
     * Вычислить интенсивности в вершинах
     */
    void calc_intencities();

    /*
     * Установить положение как "изменившееся" (заставить отрисовать сцену полностью)
     */
    void set_changed();


    Vector light_dir;           // Направление света (источник в бесконечности)
    Camera *cam;                // Камера (основная)
    Camera *reflcam;            // Камера (для отражения в воде)

    QGraphicsScene *scene;      // Сцена Qt

private:

    /*
     * Закрасить полигон (с ненулевой прозрачностью)
     * p - вершины
     * i - интенсивности
     * c_ - показатель прозрачности полигона
     */
    void triangle(const struct PolyVecs &p, struct PolyI i, double c_);

    /*
     * Закрасить полигон текстурой
     * p - вершины
     * i_ - интенсивности
     * uv - UV-координаты вершин полигона
     * tex - текстура
     * usez - если false, то рисуется, не затрагивая Z-буфер (для скайбокса)
     */
    void triangle(const struct PolyVecs &p, Vector i_, TrPolygon uv, Texture *tex, bool usez = true);

    int w;                      // Ширина
    int h;                      // Высота
    ImageItem *img;             // Изображение
    Skybox *skybox;             // Модель скайбокса
    WaterModel model;           // Модель воды
    PoolModel pool_model;       // Модель стенок бассейна
    Matrix projMatr;            // Матрица перспективного искажения
    Matrix viewMatr;            // Матрица перехода к СК камеры
    Matrix viewPort;            // Матрица перехода к экранным координатам
    int *zbuf;                  // Z-буфер
    Matrix ibuf;                // Буфер интенсивностей непрозрачных полигонов
    Matrix ibuf_transparent;    // Буфер интенсивностей прозрачных полигонов
    double *c_tr;               // Буфер взвешенных коэффициентов интенсивностей прозрачных полигонов
    Texture *pool_tex;          // Текстура бассейна (временно)
    bool changed;               // Если true, то изменилось положение камеры, сцена отрисуется полностью заново
    Vector light_color;         // Цвет освещения


};

#endif // Scene_H
