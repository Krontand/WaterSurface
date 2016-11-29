#include "poolmodel.h"

PoolModel::PoolModel()
{
}

PoolModel::PoolModel(double x1, double x2, double z1, double z2)
{
    this->tex = new ImageItem(":/pool/pool.bmp");

    const int xv = 4;
    const int zv = 2;
    xvert = xv;
    zvert = zv;
    surf = Matrix(xv*zv, 4);
    surf_screen = Matrix(xv*zv, 4);

    for(int j = 0; j < xv; ++j)
    {
        surf[j][1] = 1.2 * -0.1;
        surf[j][3] = 1;
        surf[j+xv][1] = 1.2 * -0.1 + .3;
        surf[j+xv][3] = 1;
    }

    x1 -= 0.002;
    x2 += 0.002;
    z1 -= 0.002;
    z2 += 0.002;

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

    ufloor1 = {0.05, 0.95, 0.95};
    vfloor1 = {0.05, 0.05, 0.95};
    ufloor2 = {0.05, 0.05, 0.95};
    vfloor2 = {0.05, 0.95, 0.95};

    uwall1 = {0.05, 0.45, 0.45};
    vwall1 = {0.05, 0.05, 0.95};
    uwall2 = {0.05, 0.05, 0.45};
    vwall2 = {0.05, 0.95, 0.95};


    surf_norms = Matrix(2 * xv * zv + 2, 3);
    vert_norms = Matrix(xv * zv, 3);
    surf_deform_norms = Matrix(2*xv*zv+2, 3);
    i = Matrix(xv*zv, 3);
    color = Vector({200, 200, 200});

    surf_norms[1] = std::valarray<double>({0., 0., 1.});
    surf_norms[3] = std::valarray<double>({1., 0., 0.});
    surf_norms[5] = std::valarray<double>({0., 0., -1.});
    surf_norms[7] = std::valarray<double>({-1., 0., 0.});
    surf_norms[9] = std::valarray<double>({0., -1., 0.});
    surf_norms[0] = std::valarray<double>({0., 0., 1.});
    surf_norms[2] = std::valarray<double>({1., 0., 0.});
    surf_norms[4] = std::valarray<double>({0., 0., -1.});
    surf_norms[6] = std::valarray<double>({-1., 0., 0.});
    surf_norms[8] = std::valarray<double>({0., -1., 0.});
    this->init_polygons();
}

PoolModel::~PoolModel()
{

}

void PoolModel::calc_normals()
{

}

void PoolModel::setnormals()
{
    for (int k = 0; k < 3; ++k)
    {
        surf_deform_norms[2*k] = -V(get_normal(V(surf_screen[k]),
            V(surf_screen[k+1]), V(surf_screen[k+xvert+1])));
    }
    surf_deform_norms[6] = -V(get_normal(V(surf_screen[3]),
        V(surf_screen[0]), V(surf_screen[4])));
    surf_deform_norms[8] = -V(get_normal(V(surf_screen[4]),
                              V(surf_screen[5]), V(surf_screen[6])));
}

void PoolModel::init_polygons()
{
    TrPolygon buf;
    buf.u[0] = .05; buf.u[1] = .45; buf.u[2] = .45;
    buf.v[0] = .05; buf.v[1] = .05; buf.v[2] = .95;

    for (int i = 0; i < 4; ++i)
    {
        buf.u[1] = .45;
        buf.v[1] = .05;
        buf.vert[0] = i;
        buf.vert[1] = i+4;
        buf.vert[2] = i+5;
        this->polygons.push_back(buf);
        buf.u[1] = .05;
        buf.v[1] = .95;
        buf.vert[0] = i;
        buf.vert[1] = i+1;
        buf.vert[2] = i+5;
        this->polygons.push_back(buf);
    }
    this->polygons[7].vert[1] = 0;
    this->polygons[7].vert[2] = 4;
    this->polygons[6].vert[2] = 4;

    buf.u[0] = .05; buf.u[1] = .95; buf.u[2] = .95;
    buf.v[0] = .05; buf.v[1] = .05; buf.v[2] = .95;

    buf.vert[0] = 4;
    buf.vert[1] = 5;
    buf.vert[2] = 6;
    this->polygons.push_back(buf);
    buf.u[1] = .05;
    buf.v[1] = .95;
    buf.vert[0] = 4;
    buf.vert[1] = 7;
    buf.vert[2] = 6;
    this->polygons.push_back(buf);
}
