#ifndef SKYBOX_H
#define SKYBOX_H

#include <QVector>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <iostream>


#include "model.h"



class Skybox : public Model
{
public:
    Skybox();

    void calc_normals() {}

    /*
     * Перемещение скайбокса в заданную точку
     */
    void moveto(Vector c);

private:
    void init_polygons();       // Загрузка полигонов

    Vector center;              // Центр скайбокса (смещен вверх относительно реального центра куба)
    double halfsize;            // Половина размера стороны скайбокса
};

#endif // SKYBOX_H
