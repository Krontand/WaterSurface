#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::setscreensurf(const Matrix &m)
{
    #pragma omp parallel for
    for (int i = 0; i < surf.size()/4; ++i)
    {
        surf_screen[i] = m * V(surf[i]);
    }
    for (int i = 0; i < surf.size()/4; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {

            surf_screen[i][j] /= surf_screen[i][3];
        }
    }
}

struct PolyVecs Model::vert(int num)
{
    struct PolyVecs p;
    p.a = surf_screen[polygons[num].vert[0]];
    p.b = surf_screen[polygons[num].vert[1]];
    p.c = surf_screen[polygons[num].vert[2]];
    return p;
}

struct PolyI Model::intencity(int num)
{
    struct PolyI p;
    p.ia = V(i[polygons[num].vert[0]]);
    p.ib = V(i[polygons[num].vert[1]]);
    p.ic = V(i[polygons[num].vert[2]]);
    return p;
}




Vector Model::get_normal(const Vector &a, const Vector &b, const Vector &c)
{
    Vector n(3);
    Vector v1(3);
    Vector v2(3);

    v1[0] = a[0] - b[0];
    v1[1] = a[1] - b[1];
    v1[2] = a[2] - b[2];

    v2[0] = b[0] - c[0];
    v2[1] = b[1] - c[1];
    v2[2] = b[2] - c[2];

    n[0] = (v1[1] * v2[2] - v1[2] * v2[1]);
    n[1] = (v1[2] * v2[0] - v1[0] * v2[2]);
    n[2] = (v1[0] * v2[1] - v1[1] * v2[0]);
    double wrki = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
    n[0] = n[0] / wrki;
    n[1] = n[1] / wrki;
    n[2] = n[2] / wrki;
    return n;
}
