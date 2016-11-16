#ifndef POOLMODEL_H
#define POOLMODEL_H

#include "model.h"

class PoolModel : public Model
{
public:
    PoolModel();
    PoolModel(double x1, double x2, double z1, double z2);
    ~PoolModel();

    /*
     * Расчет нормалей в вершинах
     */
    void calc_normals();
    Matrix surf_deform_norms;

    Vector uwall1;      // UV-координаты полигонов боковых стенок
    Vector vwall1;
    Vector uwall2;
    Vector vwall2;

    Vector ufloor1;      // UV-координаты полигонов пола
    Vector vfloor1;
    Vector ufloor2;
    Vector vfloor2;

    int zvert; // Количество вершин по Z
    /*
     * Расчет нормалей преобразованного объекта
     */
    void setnormals();

private:
    void init_polygons();

};

#endif // POOLMODEL_H
