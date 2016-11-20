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

    void calc_normals() {};
    void setnormals();
    void moveto(Vector c);


    Matrix surf_deform_norms;    
private:
    void init_polygons();


    Vector center;


    double halfsize;
};

#endif // SKYBOX_H
