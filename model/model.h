#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include "geometry/geometry.h"
#include "texture.h"
#include "camera.h"

#define RCOLOR 0
#define GCOLOR 149
#define BCOLOR 182

#define YS -.1
#define W 1.985


typedef struct
{
    int vert[3];        // Номера вершин
    float u[3];         // U-координаты
    float v[3];         // V-координаты
} TrPolygon;

struct PolyVecs
{
    Vector a;
    Vector b;
    Vector c;
} ;

struct PolyI
{
    Vector ia;
    Vector ib;
    Vector ic;
};

//triangle(model.vert(i), model.intencity(i), model.transparent);

class Model
{
public:
    Model();
    ~Model();

    /*
     * Расчет экранных координат по готовой матрице преобразования m
     */
    void setscreensurf(const Matrix &m);

    void apply_matrix(const Matrix &m);

    /*
     * Расчет нормалей в вершинах
     */
    virtual void calc_normals() = 0;


    struct PolyVecs vert(int num);
    struct PolyVecs _vert(int num);
    struct PolyI intencity(int num);

    double transparent;                   // Коэффициент прозрачности материала модели
    Matrix surf;                          // Матрица вершин
    Matrix surf_screen;                   // Матрица экранных координат вершин
    std::vector<TrPolygon> polygons;      // Полигоны модели
    Matrix i;                             // Интенсивности света в вершинах
    Vector color;                         // Цвет
    Matrix vert_norms;                    // Координаты нормалей к вершинам
    Matrix surf_norms;                    // Координаты нормалей к треугольникам

    int xvert;

    std::vector<TrPolygon> clip_polys;    // Полигоны модели
    Texture *tex;

    Matrix vert_clip;
    void clip(Camera *cam);
    void setclipview(const Matrix &m);
    PolyVecs clipvert(int num);
protected:
    /*
     * Расчет нормали к треугольнику abc
     */
    Vector get_normal(const Vector &a, const Vector &b, const Vector &c);
    Matrix n;
    Matrix o;
    void clip_poly(int n, int &num, Matrix &vert);
    int clip_plane(Matrix &dst, const Matrix &src, int num, Vector n, Vector o);
    int verts;

};

#endif // MODEL_H
