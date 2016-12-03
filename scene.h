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
#include "renderer.h"
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
     * Нарисовать сцену
     */
    void render();

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

    void disturb(int x, int y);

    void scale(float k);


    Vector light_dir;           // Направление света (источник в бесконечности)
    Camera *cam;                // Камера (основная)
    Camera *reflcam;            // Камера (для отражения в воде)

    QGraphicsScene *scene;      // Сцена Qt

private:
    int w;
    int h;

    int wrefl;                  // Ширина изображения отражения
    int hrefl;                  // Высота изображения отражения

    float k;

    ImageItem *img;

    Skybox *skybox;             // Модель скайбокса
    WaterModel *model;           // Модель воды
    PoolModel *pool_model;       // Модель стенок бассейна

    bool changed;               // Если true, то изменилось положение камеры, сцена отрисуется полностью заново
    Vector light_color;         // Цвет освещения

    Renderer *renderer;
    Renderer *refl_renderer;

    const double amax = 0.7;


};

#endif // Scene_H
