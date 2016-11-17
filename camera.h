#ifndef CAMERA_H
#define CAMERA_H

#include "geometry/geometry.h"

class Camera
{
public:
    Camera(double x, double y, double z);

    Vector eye;
    Vector center;
    Vector up;

    Vector eye_s;
    double anglex;
    double angley;
    double dist;
};

#endif // CAMERA_H
