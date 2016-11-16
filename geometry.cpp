#include "geometry.h"


Vector cross(const Vector &a, const Vector &b)
{
    Vector c(3);
    c[0] = a[1]*b[2] - a[2]*b[1];    // y1z2-z1y2
    c[1] = a[2]*b[0] - a[0]*b[2];    // z1x2-x1z2
    c[2] = a[0]*b[1] - a[1]*b[0];    // x1y2-y1x2
    return c;
}

void identity4(Matrix &a)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                a[i][j] = 1;
            else
                a[i][j] = 0;
        }

}

void normalize(Vector &v)
{
    v = v / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

Vector norm(const Vector &v)
{
    Vector n(3);
    n = v / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return n;
}


Vector barycentric(const Vector &a, const Vector &b, const Vector &c, int x, int y)
{
    Vector v1(3);
    Vector v2(3);
    v1[0] = b[0] - a[0];
    v1[1] = c[0] - a[0];
    v1[2] = a[0] - x;
    v2[0] = b[1] - a[1];
    v2[1] = c[1] - a[1];
    v2[2] = a[1] - y;
    Vector bar = cross(v1, v2);

    if (std::abs(b[2]) < 1)
    {
        bar[0] = -1;
        return bar;
    }

    bar[0] = bar[0] / bar[2];
    bar[1] = bar[1] / bar[2];
    bar[2] = bar[2] / bar[2];

    return bar;

}

void swap_(int *a, int *b)
{
      int t;
      t = *a;
      *a = *b;
      *b = t;
}

void swap_(double *a, double *b)
{
    double t;
    t = *a;
    *a = *b;
    *b = t;
}

void swap_(float *a, float *b)
{
    float t;
    t = *a;
    *a = *b;
    *b = t;
}

void dswap(double *a, double *b)
{
    double t;
    t = *a;
    *a = *b;
    *b = t;
}

double dot(const Vector &a, const Vector &b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

Vector persp_norm(const Vector &cam, const Vector &norm)
{
    double c = sqrt(cam[0] * cam[0] + cam[1] * cam[1] + cam[2] * cam[2]);
    Vector n(3);
    n[0] = norm[0];
    n[1] = norm[1];
    n[2] = norm[2] + 1/c;
    return n;

}

void transpose(Matrix &m)
{
    for (int i = 0; i < m.rowno(); i++)
        for (int j = i; j < m.colno(); j++)
            swap_(&m[i][j], &m[j][i]);

}
