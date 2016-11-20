#ifndef CAMERA_H
#define CAMERA_H

#include "geometry/geometry.h"

class Camera
{
public:
    Camera(double x, double y, double z);

    Vector eye;     // Позиция камеры

    Vector center;  // Точка взгляда камеры (target)

    Vector up;      // Вектор, всегда смотрящий "вверх" - нужен для корректного перехода к СК камеры

    Vector eye_s;   // Начальные координаты камеры
    double anglex;  // Поворот в горизонтальной плоскости
    double angley;  // Поворот в вертикальной плоскости
    double dist;    // Расстояние от центра координат мировой СК
};

#endif // CAMERA_H
