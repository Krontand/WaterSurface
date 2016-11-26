#include "cammod.h"

CamMod::CamMod(QObject *parent) : QObject(parent)
{
    xpos = -1;
    ypos = -1;
}

void CamMod::mouseEvent(QMouseEvent *pe)
{
    if (xpos == -1)
    {
        xpos = pe->x();
        ypos = pe->y();
        return;
    }
    if (xpos != pe->x())
        rotatey((pe->x() - xpos)/50.0);
    if (ypos != pe->y())
        rotatex((ypos - pe->y())/50.0);
    xpos = pe->x();
    ypos = pe->y();

    scene->cam->eye[2] = scene->cam->dist * sin(scene->cam->anglex) * cos(scene->cam->angley);
    scene->cam->eye[0] = scene->cam->dist * sin(scene->cam->anglex) * sin(scene->cam->angley);
    scene->cam->eye[1] = scene->cam->dist * cos(scene->cam->anglex);
}

void CamMod::rotatey(double angle)
{
    scene->cam->angley += angle;
    scene->set_changed();
}

void CamMod::rotatex(double angle)
{
    scene->cam->anglex += angle;
    scene->set_changed();
}

void CamMod::drawimage()
{
    scene->render();
}

void CamMod::updatescene()
{
    scene->updheights();
    scene->calc_normals();
    scene->calc_intencities();
    scene->render();
}

void CamMod::rand_disturb()
{
    scene->rand_disturb();
}
