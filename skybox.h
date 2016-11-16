#ifndef SKYBOX_H
#define SKYBOX_H

#include <QVector>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <iostream>


#include "model.h"
#include "texture.h"
#include "camera.h"


class Skybox : public Model
{
public:
    Skybox();

    void calc_normals() {};
    void setnormals();
    void moveto(Vector c);
    void clip(Camera cam);
    struct PolyVecs vert(int num);
    void setclipview(const Matrix &m);

    Matrix surf_deform_norms;
    std::vector<TrPolygon> clip_polys;    // Полигоны модели
    Texture *tex;

    Matrix vert_clip;

private:
    void init_polygons();
    void clip_poly(int n, int &num, Matrix &vert);
    int clip_plane(Matrix &dst, const Matrix &src, int num, Vector n, Vector o);

    Vector center;
    Matrix n;
    Matrix o;
    int verts;
};

#endif // SKYBOX_H
