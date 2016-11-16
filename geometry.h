#ifndef GEOMETRY
#define GEOMETRY

#include <cmatrix>
#include "math.h"
#include "omp.h"

typedef techsoft::matrix<double> Matrix;
typedef techsoft::matrix<int> IMatrix;
typedef techsoft::matrix<float> FMatrix;
typedef std::valarray<double>    Vector;

Vector cross(const Vector &a, const Vector &b);
Vector barycentric(const Vector &a, const Vector &b, const Vector &c, int x, int y);
void identity4(Matrix &a);
void normalize(Vector &v);
Vector norm(const Vector &v);
double dot(const Vector &a, const Vector &b);
void transpose(Matrix &m);


Vector persp_norm(const Vector &cam, const Vector &norm);

void swap_(int *a, int *b);
void swap_(double *a, double *b);
void swap_(float *a, float *b);
void dswap(double *a, double *b);

#define V(x) (static_cast<Vector>(x))
#define FV(x) (static_cast<std::valarray<float> >(x))

#endif // GEOMETRY

