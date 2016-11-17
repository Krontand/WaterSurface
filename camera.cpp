#include "camera.h"

Camera::Camera(double x, double y, double z)
{
    eye = Vector(3);
    center = Vector(3);
    up = Vector(3);
    anglex = 0;
    angley = 0;

    center[0] = 0;
    center[1] = 0;
    center[2] = 0;
    up[0] = 0;
    up[1] = 1;
    up[2] = 0;

    eye[0] = x;
    eye[1] = y;
    eye[2] = z;
    eye_s = eye;
    dist = sqrt(dot(eye, eye));

}

