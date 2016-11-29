#include "watermodel.h"
#include "geometry/geometry.h"

WaterModel::~WaterModel()
{

}


WaterModel::WaterModel()
{

}

WaterModel::WaterModel(double x)
{
    xvert = (int)(x);

    int x2 = x / 2;

    surf = Matrix(xvert*xvert + (xvert - 2) * 4, 4);
    surf_screen = Matrix(xvert*xvert + (xvert - 2) * 4, 4);

    int k = 0;
    for(int i = -x2; i < x2; ++i)
    {
        for(int j = -x2; j < x2; ++j)
        {
            surf[k][0] = i/x;
            surf[k][1] = ybase;
            surf[k][2] = j/x;
            surf[k][3] = 1;
            k++;
        }
    }

    transparent = 0.8;

    int n = xvert*xvert;
    //Левая стенка
    for(int l = 1; l < xvert-1; l++)
    {
        surf[n + l - 1][0] = surf[l+xvert][0];
        surf[n + l - 1][1] = 1.2 * ybase + .3;
        surf[n + l - 1][2] = surf[l+xvert][2];
        surf[n + l - 1][3] = surf[l+xvert][3];
    }
    //Задняя стенка
    for(int l = 1; l < xvert-1; l++)
    {
        surf[n + xvert + l - 3][0] = surf[xvert * l + 1][0];
        surf[n + xvert + l - 3][1] = 1.2 * ybase + .3;
        surf[n + xvert + l - 3][2] = surf[xvert * l + 1][2];
        surf[n + xvert + l - 3][3] = surf[xvert * l + 1][3];
    }
    //Правая стенка
    for(int l = 1; l < xvert-1; l++)
    {
        surf[n + 2*xvert + l - 5][0] = surf[n - 2*xvert + l][0];
        surf[n + 2*xvert + l - 5][1] = 1.2 * ybase + .3;
        surf[n + 2*xvert + l - 5][2] = surf[n - 2*xvert + l][2];
        surf[n + 2*xvert + l - 5][3] = surf[n - 2*xvert + l][3];
    }
    //Передняя стенка
    for(int l = 1; l < xvert-1; l++)
    {
        surf[n + 3*xvert + l - 7][0] = surf[xvert - 2 + xvert * l][0];
        surf[n + 3*xvert + l - 7][1] = 1.2 * ybase + .3;
        surf[n + 3*xvert + l - 7][2] = surf[xvert - 2 + xvert * l][2];
        surf[n + 3*xvert + l - 7][3] = surf[xvert - 2 + xvert * l][3];
    }

    i_wall = Vector(3);
    i_wall[0] = rcolor*.3;
    i_wall[1] = gcolor*.3;
    i_wall[2] = bcolor*.3;


    H1 = Vector(ybase/1.6, x*x);
    H2 = Vector(ybase/1.6, x*x);

    surf_norms = Matrix(2 * xvert * xvert, 3);
    vert_norms = Matrix(xvert * xvert, 3);
    i = Matrix(xvert*xvert, 3);
    color = Vector(3);

    color[0] = rcolor;
    color[1] = gcolor;
    color[2] = bcolor;

    angle = 0;
    init_polygons();
}

void WaterModel::init_polygons()
{
    int xv = xvert;
    TrPolygon buf;
    for (int i = 1; i < xv-2; ++i)
    {
        for (int j = 1; j < xv-2; ++j)
        {
            int k = i*xv+j;

            buf.vert[0] = k;
            buf.vert[1] = k+xv;
            buf.vert[2] = k+xv+1;
            buf.u[0] = -(this->surf[k][0]) * 0.7 + 0.5;
            buf.u[1] = -(this->surf[k+xv][0]) * 0.7 + 0.5;
            buf.u[2] = -(this->surf[k+xv+1][0]) * 0.7 + 0.5;
            buf.v[0] = -(this->surf[k][2]) * 0.7 + 0.5;
            buf.v[1] = -(this->surf[k+xv][2]) * 0.7 + 0.5;
            buf.v[2] = -(this->surf[k+xv+1][2]) * 0.7 + 0.5;
            polygons.push_back(buf);
            buf.vert[0] = k;
            buf.vert[1] = k+1;
            buf.vert[2] = k+xv+1;
            buf.u[0] = -(this->surf[k][0]) * 0.7 + 0.5;
            buf.u[1] = -(this->surf[k+1][0]) * 0.7 + 0.5;
            buf.u[2] = -(this->surf[k+xv+1][0]) * 0.7 + 0.5;
            buf.v[0] = -(this->surf[k][2]) * 0.7 + 0.5;
            buf.v[1] = -(this->surf[k+1][2]) * 0.7 + 0.5;
            buf.v[2] = -(this->surf[k+xv+1][2]) * 0.7 + 0.5;
            polygons.push_back(buf);
        }

        buf.u[0] = 0;
        buf.u[1] = 0;
        buf.u[2] = 0;
        buf.v[0] = 0;
        buf.v[1] = 0;
        buf.v[2] = 0;
        int n = xv*xv;
        for(int l = 1; l < xv - 2; l++)
        {
            buf.vert[0] = l + xv;
            buf.vert[1] = l + n - 1;
            buf.vert[2] = l + n;
            wallpoly.push_back(buf);
            buf.vert[0] = l + xv;
            buf.vert[1] = l + xv + 1;
            buf.vert[2] = l + n;
            wallpoly.push_back(buf);        // Левая стенка (6)
        }
        for(int l = 1; l < xv - 2; l++)
        {
            buf.vert[0] = l*xv + 1;
            buf.vert[1] = xv + l + n - 3;
            buf.vert[2] = xv + l + n - 2;
            wallpoly.push_back(buf);
            buf.vert[0] = l*xv + 1;
            buf.vert[1] = xv * l + 1 + xv;
            buf.vert[2] = xv + l + n - 2;
            wallpoly.push_back(buf);        // Задняя стенка (4)
        }
        for(int l = 1; l < xv - 2; l++)
        {
            buf.vert[0] = n - 2*xv + l;
            buf.vert[1] = n + 2*xv + l - 5;
            buf.vert[2] = n + 2*xv + l - 4;
            wallpoly.push_back(buf);
            buf.vert[0] = n - 2*xv + l;
            buf.vert[1] = n - 2*xv + l + 1;
            buf.vert[2] = n + 2*xv + l - 4;
            wallpoly.push_back(buf);        // Правая стенка (2)
        }
        for(int l = 1; l < xv - 2; l++)
        {
            buf.vert[0] = xv-2+xv*l;
            buf.vert[1] = n+3*xv+l-7;
            buf.vert[2] = n+3*xv+l-6;
            wallpoly.push_back(buf);
            buf.vert[0] = xv-2+xv*l;
            buf.vert[1] = 2*xv-2+xv*l;
            buf.vert[2] = n+3*xv+l-6;
            wallpoly.push_back(buf);        // Передняя стенка (0)
        }
    }             
}

struct PolyVecs WaterModel::wallvert(int num)
{
    struct PolyVecs p;
    p.a = surf_screen[wallpoly[num].vert[0]];
    p.b = surf_screen[wallpoly[num].vert[1]];
    p.c = surf_screen[wallpoly[num].vert[2]];
    return p;
}

void WaterModel::rotateuv(double angle)
{
    double rotation = angle - this->angle;
    this->angle = angle;
    double cosa = cos(rotation);
    double sina = sin(rotation);

    TrPolygon buf;

    for (int i = 0; i < this->polygons.size(); i++)
    {
        buf.u[0] = 0.5 + (polygons[i].u[0] - 0.5) * cosa - (polygons[i].v[0] - 0.5) * sina;
        buf.u[1] = 0.5 + (polygons[i].u[1] - 0.5) * cosa - (polygons[i].v[1] - 0.5) * sina;
        buf.u[2] = 0.5 + (polygons[i].u[2] - 0.5) * cosa - (polygons[i].v[2] - 0.5) * sina;

        buf.v[0] = 0.5 + (polygons[i].v[0] - 0.5) * cosa + (polygons[i].u[0] - 0.5) * sina;
        buf.v[1] = 0.5 + (polygons[i].v[1] - 0.5) * cosa + (polygons[i].u[1] - 0.5) * sina;
        buf.v[2] = 0.5 + (polygons[i].v[2] - 0.5) * cosa + (polygons[i].u[2] - 0.5) * sina;

        polygons[i] = buf;
    }
}

void WaterModel::updheights()
{
    int k = xvert+1;
    for (int i = 2; i < xvert; ++i)
    {
        for (int j = 2; j < xvert; ++j)
        {
            surf[k][1] = (1 - w)*H1[k] + w / 4.0 * (H2[k+1] + H2[k-1] + H2[k+xvert] + H2[k-xvert]);
            k++;
        }
        k +=2;
    }
    H1 = H2;
    for (int i = 0; i < H2.size(); ++i)
        H2[i] = surf[i][1];
}



void WaterModel::calc_normals()
{
    int k = 0;
     // Считаем нормали к граням
    for (int i = 1; i < xvert; ++i)
    {
        for (int j = 1; j < xvert; ++j)
        {
            surf_norms[2*k] = -V(get_normal(V(surf[k]),
                       V(surf[k+xvert+1]), V(surf[k+xvert])));
            surf_norms[2*k+1] = -V(get_normal(V(surf[k]),
                    V(surf[k+1]), V(surf[k+xvert+1])));
            k++;
        }
        k += 1;
    }

    // Считаем нормали к вершинам
    k = xvert + 1;
    for (int i = 2; i < xvert; ++i)
    {
        for (int j = 2; j < xvert; ++j)
        {
            vert_norms[k] = V(surf_norms[2*k]) + V(surf_norms[2*k+1])
                    + V(surf_norms[2*(k-xvert)]) + V(surf_norms[2*(k-xvert)-1]) +
                    V(surf_norms[2*(k-xvert-1)]);
            k++;
        }
        k += 2;

    }

}

