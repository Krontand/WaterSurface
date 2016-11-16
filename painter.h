#ifndef Scene_H
#define Scene_H

#include "imageitem.h"
#include "watermodel.h"
#include "camera.h"
#include "geometry.h"
#include "math.h"
#include "poolmodel.h"
#include "texture.h"
#include "skybox.h"
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

    void setviewmatr();
    void setprojmatr();
    void setViewPort(int x, int y, int w, int h);

    void drawimage();
    void updheights();
    void rand_disturb();    
    void calc_normals();
    void calc_intencities();
    void set_changed();

    Vector light_dir;
    Camera cam;
    QGraphicsScene *scene;

private:    
    void triangle(const struct PolyVecs &p, struct PolyI i, double c_);

    int w;
    int h;
    ImageItem *img;
    Skybox *skybox;
    WaterModel model;
    PoolModel pool_model;
    Matrix projMatr;
    Matrix viewMatr;
    Matrix viewPort;
    int *zbuf;
    Matrix ibuf;
    Matrix ibuf_transparent;
    double *c_tr;
    Texture *pool_tex;
    bool changed;
    Vector light_color;

    void triangle(const struct PolyVecs &p, Vector i_, TrPolygon uv, Texture *tex, bool usez = true);
   // void triangle(Vector a, Vector b, Vector c, Vector i_, Vector ut, Vector vt);
};

#endif // Scene_H
