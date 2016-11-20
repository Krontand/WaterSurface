#ifndef GEOMETRY
#define GEOMETRY

#include <geometry/cmatrix>
#include "math.h"
#include "omp.h"

typedef techsoft::matrix<double> Matrix;
typedef techsoft::matrix<int> IMatrix;
typedef techsoft::matrix<float> FMatrix;
typedef std::valarray<double>    Vector;


// Векторное произведение
Vector cross(const Vector &a, const Vector &b);

// Заполняет матрицу 4*4 как единичную
void identity4(Matrix &a);

// Нормирование вектора
void normalize(Vector &v);
Vector norm(const Vector &v);

// Скалярное произведение векторов
double dot(const Vector &a, const Vector &b);

// Транпонирование матрицы
void transpose(Matrix &m);

// Вектор нормали после искажения перспективой
Vector persp_norm(const Vector &cam, const Vector &norm);

void swap_(int *a, int *b);
void swap_(double *a, double *b);
void swap_(float *a, float *b);
void dswap(double *a, double *b);

#define V(x) (static_cast<Vector>(x))
#define FV(x) (static_cast<std::valarray<float> >(x))

#endif // GEOMETRY

