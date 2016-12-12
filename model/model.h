#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include "geometry/geometry.h"
#include "imageitem.h"
#include "camera.h"


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
};

struct PolyI
{
    Vector ia;
    Vector ib;
    Vector ic;
};

class Model
{
public:
    Model();
    ~Model();

    /*
     * Расчет экранных координат по готовой матрице преобразования m
     */
    void setscreensurf(const Matrix &m);

    /*
     * Применить матрицу m к исходным координатам модели
     */
    void apply_matrix(const Matrix &m);

    /*
     * Применить матрицу m к вершинам модели, отсеченной пирамидой видимости
     */
    void setclipview(const Matrix &m);

    /*
     * Расчет нормалей в вершинах
     */
    virtual void calc_normals() = 0;

    /*
     * Возвращает вершины полигона с заданным номером (экранные, неотсеченные)
     */
    struct PolyVecs vert(int num);

    /*
     * Возвращает вершины полигона с заданным номером (экранные, отсеченные)
     */
    PolyVecs clipvert(int num);

    /*
     * Возвращает интенсивности вершин полигона с заданным номером
     */
    struct PolyI intencity(int num);

    /*
     * Отсечение пирамидой видимости камеры cam
     */
    void clip(Camera *cam);



    double transparent;                   // Коэффициент прозрачности материала модели
    Matrix surf;                          // Матрица вершин
    Matrix surf_screen;                   // Матрица экранных координат вершин
    std::vector<TrPolygon> polygons;      // Полигоны модели
    Matrix i;                             // Интенсивности света в вершинах
    Vector color;                         // Цвет
    Matrix vert_norms;                    // Координаты нормалей к вершинам
    Matrix surf_norms;                    // Координаты нормалей к треугольникам
    std::vector<TrPolygon> clip_polys;    // Полигоны модели после отсечения пирамидой видимости
    ImageItem *tex;                       // Текстура модели
    Matrix vert_clip;                     // Вершины модели после отсечения

    int xvert;
protected:
    /*
     * Расчет нормали к треугольнику abc
     */
    Vector get_normal(const Vector &a, const Vector &b, const Vector &c);


    Matrix n;   // Нормали пирамиды видимости
    Matrix o;   // Точки, принадлежащие сторонам пирамиды видимости

    /*
     * Отсечение треугольного полигона пирамидой видимости.
     * n - номер полигона
     * num - количество вершин в итоговом полигоне.
     */
    void clip_poly(int n, int &num, Matrix &vert);

    /*
     * Отсечение полигона стороной пирамиды видимости
     * Результат - количество вершин в итоговом полигоне
     * dst - итоговые вершины
     * src - исходные вершины
     * num - количество вершин в исходном полигоне
     * n - нормаль к плоскости отсечения (внутренняя для пирамиды видимости
     * o - точка, принадлежащая плоскости отсечения
     */
    int clip_plane(Matrix &dst, const Matrix &src, int num, Vector n, Vector o);
    int verts;

};

#endif // MODEL_H
