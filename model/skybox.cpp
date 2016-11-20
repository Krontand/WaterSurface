#include "skybox.h"

Skybox::Skybox()
{
    const int xv = 4;
    const int zv = 2;
    xvert = xv;
    surf = Matrix(xv*zv, 4);
    surf_screen = Matrix(xv*zv, 4);

    halfsize = 0.05;

    tex = new Texture(":/sky/skybox.bmp");
    for(int j = 0; j < xv; ++j)
    {
        surf[j][1] = halfsize + halfsize/3;
        surf[j][3] = 1;
        surf[j+xv][1] = -halfsize + halfsize/3;
        surf[j+xv][3] = 1;
    }

    float x1 = -halfsize;
    float x2 = halfsize;
    float z1 = -halfsize;
    float z2 = halfsize;

    surf[0][0] = x1;
    surf[0][2] = z2;
    surf[1][0] = x2;
    surf[1][2] = z2;
    surf[2][0] = x2;
    surf[2][2] = z1;
    surf[3][0] = x1;
    surf[3][2] = z1;
    surf[4][0] = x1;
    surf[4][2] = z2;
    surf[5][0] = x2;
    surf[5][2] = z2;
    surf[6][0] = x2;
    surf[6][2] = z1;
    surf[7][0] = x1;
    surf[7][2] = z1;

    i = Matrix(xv*zv, 3);
    color = Vector({200, 200, 200});

    center = Vector(3);
    center[0] = 0;
    center[1] = 0;
    center[2] = 0;

    this->init_polygons();

    n = Matrix(6, 3);
    o = Matrix(6, 3);
    vert_clip = Matrix(72, 5);
}

void Skybox::init_polygons()
{
    TrPolygon buf;

    QFile f(":/skybox.dat");

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&f);

    const QStringList &aStringList = in.readAll().split("\n");

    size_t x = aStringList.size() - 1;

    for (size_t i = 0; i < x; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
            buf.vert[j] = aStringList.at(i).split(" ").at(j).toInt();
        for (size_t j = 3; j < 6; ++j)
            buf.u[j-3] = aStringList.at(i).split(" ").at(j).toFloat();
        for (size_t j = 6; j < 9; ++j)
            buf.v[j-6] = aStringList.at(i).split(" ").at(j).toFloat();
        this->polygons.push_back(buf);
    }
}


void Skybox::setnormals()
{
    int num = this->polygons.size();
    for (int i = 0; i < num; i++)
    {
         this->surf_deform_norms[i] = -V(get_normal(
             V(surf_screen[polygons[i].vert[0]]),
             V(surf_screen[polygons[i].vert[1]]),
             V(surf_screen[polygons[i].vert[2]])));
    }
}

void Skybox::moveto(Vector c)
{
    double dx = c[0] - center[0];
    double dy = c[1] - center[1];
    double dz = c[2] - center[2];
    for (int i = 0; i < surf.size()/4; i++)
    {
        surf[i][0] += dx;
        surf[i][1] += dy;
        surf[i][2] += dz;
    }
    center[0] = c[0]; center[1] = c[1]; center[2] = c[2];
}
