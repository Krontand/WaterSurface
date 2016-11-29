#ifndef WATERMODEL_H
#define WATERMODEL_H

#include "model.h"

class WaterModel : public Model
{
public:
    WaterModel();
    WaterModel(double x);
    ~WaterModel();

    /*
     * Расчет высот в следующий момент времени
     */
    void updheights();

    /*
     * Расчет нормалей в вершинах
     */
    void calc_normals();

    /*
     * Получение вершин заданного полигона стенки воды
     */
    struct PolyVecs wallvert(int num);

    void rotateuv(double angle);

    Vector i_wall;                        // Интенсивность света стенок воды

    int xvert;                            // Количество вершин в сетке по X (оно же по Z)

    std::vector<TrPolygon> wallpoly;      // Полигоны стенки воды

    Vector H1; // Высоты поверхности в предыдущий момент времени
    Vector H2; // Высоты поверхности в "пред-предыдущий" момент времени


private:
    /*
     * Загрузка полигонов
     */
    void init_polygons();

    const float uvscale = 0.7;
    const float uvoffset = 0.15;

    const int rcolor = 0;           // RGB-составляющие цвета воды
    const int gcolor = 149;
    const int bcolor = 182;

    const double ybase = -0.1;      // Базовая высота поверхности воды
    const double w = 1.985;         // Коэффициент вязкости

    double angle;

};

#endif // WATERMODEL_H
