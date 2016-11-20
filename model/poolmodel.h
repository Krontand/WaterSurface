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

    /*
     * Расчет нормалей преобразованного объекта
     * с учетом перспективного искажения. Нужно для определения видимости
     * стенки бассейна в итоговом изображении
     */
    void setnormals();

    Matrix surf_deform_norms;   // Нормали объекта с учетом перспективы

    Vector uwall1;              // UV-координаты полигонов боковых стенок
    Vector vwall1;
    Vector uwall2;
    Vector vwall2;

    Vector ufloor1;             // UV-координаты полигонов пола
    Vector vfloor1;
    Vector ufloor2;
    Vector vfloor2;

    int zvert;                  // Количество вершин по Z


private:
    void init_polygons();       // Загрузка полигонов

};

#endif // POOLMODEL_H
