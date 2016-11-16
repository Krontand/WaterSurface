#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"

class Camera
{
public:
    Camera();

    Vector eye;
    Vector center;
    Vector up;

    Vector eye_s;
    double anglex;
    double angley;
};

#endif // CAMERA_H
