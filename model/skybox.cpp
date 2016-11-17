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
        surf[j][1] = halfsize;
        surf[j][3] = 1;
        surf[j+xv][1] = -halfsize;
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

void Skybox::clip(Camera *cam)
{
    float zcp = sqrt(dot(this->center, this->center));
    float yd = 10;
    float yu = 10;
    float xl = 10;
    float xr = 10;

     this->n[0][0] = 0;    this->n[0][1] = 0;       this->n[0][2] = -1;
     this->n[1][0] = 0;    this->n[1][1] = -zcp-1;  this->n[1][2] = -yd;
     this->n[2][0] = 0;    this->n[2][1] = zcp+1;  this->n[2][2] = -yu;
     this->n[3][0] = -zcp-1; this->n[3][1] = 0;    this->n[3][2] = -xl;
     this->n[4][0] = zcp+1;  this->n[4][1] = 0;    this->n[4][2] = -xr;

    o[0][0] = 0;    o[0][1] = 0;    o[0][2] = zcp-.01;
    o[1][0] = 0;    o[1][1] = 0;    o[1][2] = zcp;
    o[2][0] = 0;    o[2][1] = 0;    o[2][2] = zcp;
    o[3][0] = 0;    o[3][1] = 0;    o[3][2] = zcp;
    o[4][0] = 0;    o[4][1] = 0;    o[4][2] = zcp;
/*
    this->n[0][0] = 0;    this->n[1][0] = 0;       this->n[2][0] = -1;
    this->n[0][1] = 0;    this->n[1][1] = -zcp-1;  this->n[2][1] = -yd;
    this->n[0][2] = 0;    this->n[1][2] = zcp+1;  this->n[2][2] = -yu;
    this->n[0][3] = -zcp-1; this->n[1][3] = 0;    this->n[2][3] = -xl;
    this->n[0][4] = zcp+1;  this->n[1][4] = 0;    this->n[2][4] = -xr;

   o[0][0] = 0;    o[1][0] = 0;    o[2][0] = zcp-.01;
   o[0][1] = 0;    o[1][1] = 0;    o[2][1] = zcp;
   o[0][2] = 0;    o[1][2] = 0;    o[2][2] = zcp;
   o[0][3] = 0;    o[1][3] = 0;    o[2][3] = zcp;
   o[0][4] = 0;    o[1][4] = 0;    o[2][4] = zcp;
*/
    Matrix vert = Matrix(12, 5);
    int num = this->polygons.size();
    int n;
    int k = 0;
    TrPolygon p;
    this->clip_polys.clear();
    for (int i = 0; i < num; i++)
    {
        this->clip_poly(i, n, vert);
        int l = k;
        for (int j = 0; j < n; j++)
        {
            this->vert_clip[k][0] = vert[j][0];
            this->vert_clip[k][1] = vert[j][1];
            this->vert_clip[k][2] = vert[j][2];
            this->vert_clip[k][3] = 1;
            k++;
        }
        for (int j = 1; j < n - 1; j++)
        {
            p.vert[0] = l;
            p.vert[1] = l+j;
            p.vert[2] = l+j+1;
            p.u[0] = vert[0][3];
            p.u[1] = vert[j][3];
            p.u[2] = vert[j+1][3];
            p.v[0] = vert[0][4];
            p.v[1] = vert[j][4];
            p.v[2] = vert[j+1][4];
            clip_polys.push_back(p);
        }
    }
    this->verts = k;
}

PolyVecs Skybox::vert(int num)
{
    struct PolyVecs p;
    p.a = vert_clip[clip_polys[num].vert[0]];
    p.b = vert_clip[clip_polys[num].vert[1]];
    p.c = vert_clip[clip_polys[num].vert[2]];
    return p;
}

PolyVecs Skybox::_vert(int num)
{
    struct PolyVecs p;
    p.a = this->surf_screen[this->polygons[num].vert[0]];
    p.b = this->surf_screen[this->polygons[num].vert[1]];
    p.c = this->surf_screen[this->polygons[num].vert[2]];
    return p;
}

void Skybox::setclipview(const Matrix &m)
{
    #pragma omp parallel for
    for (int i = 0; i < verts; ++i)
    {
        vert_clip[i] = m * V(vert_clip[i]);
    }
    for (int i = 0; i < verts; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {

            vert_clip[i][j] /= vert_clip[i][3];
        }
    }
}

void Skybox::clip_poly(int n, int &num, Matrix &vert)
{
    num = 3;
    Matrix t1 = Matrix(12, 5);
    Matrix t2 = Matrix(12, 5);

    for (int i = 0; i < num; i++)
    {
        t2[i][0] = this->surf_screen[this->polygons[n].vert[i]][0];
        t2[i][1] = this->surf_screen[this->polygons[n].vert[i]][1];
        t2[i][2] = this->surf_screen[this->polygons[n].vert[i]][2];
        t2[i][3] = this->polygons[n].u[i];
        t2[i][4] = this->polygons[n].v[i];
    }

    for (int i = 0; i < 5; i++)
    {
        int k = clip_plane(t1, t2, num, this->n[i], this->o[i]);
        num = k;
        t2 = t1;
    }
    vert = t1;
   /* for (int i = 0; i < num; i++)
    {
        std::cout << vert[i][0] << " " << vert[i][1] << " " << vert[i][2] << " " << vert[i][3] << " " << vert[i][4] << std::endl;
    }
    std::cout << std::endl;*/

}

int Skybox::clip_plane(Matrix &dst, const Matrix &src, int num, Vector n, Vector o)
{
    int i, r;
    Vector p1, p2, tmp;
    float t1, t2;
    float k;

    normalize(n);

    r = 0;
    for (i = 0; i < num; i++)
    {
        p1 = V(src[i]);
        p2 = V(src[(i + 1) % num]);

   //     std::cout << "\n" << p1[0] << " " << p1[1] << " " << p1[2];

        tmp = p1-o;
        normalize(tmp);
        t1 = dot(p1 - o, n);
        tmp = p2-o;
        normalize(tmp);
        t2 = dot(p2 - o, n);

        if (t1 >= 0)
        {   // если начало лежит в области
            dst[r][0] = p1[0];
            dst[r][1] = p1[1];
            dst[r][2] = p1[2];
            dst[r][3] = p1[3];
            dst[r][4] = p1[4];
            r++;
        }
        // если ребро пересекает границу
        // добавляем точку пересечения
        if (((t1 >  0) && (t2 < 0)) || ((t2 >= 0) && (t1 < 0)))
        {
             k =  - dot(p1-o, n) / dot(p2-p1, n);
             dst[r][0] = p1[0] + k * (p2[0] - p1[0]);
             dst[r][1] = p1[1] + k * (p2[1] - p1[1]);
             dst[r][2] = p1[2] + k * (p2[2] - p1[2]);
             dst[r][3] = p1[3] + k * (p2[3] - p1[3]);
             dst[r][4] = p1[4] + k * (p2[4] - p1[4]);
             r++;
        }
    }
    return r;
}
