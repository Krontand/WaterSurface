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
    struct PolyVecs wallvert(int num);


    Vector i_wall;

    int xvert; // Количество вершин в сетке по X (оно же по Z)

    std::vector<TrPolygon> wallpoly;      // Полигоны модели

    Vector H1; // Высоты поверхности в предыдущий момент времени
    Vector H2; // Высоты поверхности в "пред-предыдущий" момент времени


private:
    void init_polygons();

};

#endif // WATERMODEL_H
