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
}

void CamMod::rotatey(double angle)
{
    scene->cam.angley += angle;
    angle = scene->cam.angley;
    scene->cam.eye[0] = cos(angle)*scene->cam.eye_s[0] + sin(angle)*scene->cam.eye_s[2];
    scene->cam.eye[2] = -sin(angle)*scene->cam.eye_s[0] + cos(angle)*scene->cam.eye_s[2];
    scene->set_changed();
}

void CamMod::rotatex(double angle)
{
    scene->cam.anglex += angle;
    angle = scene->cam.anglex;
    scene->cam.eye[1] = cos(angle)*scene->cam.eye_s[1] - sin(angle)*scene->cam.eye_s[2];
    scene->cam.eye[2] = sin(angle)*scene->cam.eye_s[1] + cos(angle)*scene->cam.eye_s[2];
    scene->set_changed();
}

void CamMod::drawimage()
{
    scene->drawimage();
}

void CamMod::updatescene()
{
    scene->updheights();
    scene->calc_normals();
    scene->calc_intencities();
    scene->setviewmatr();
    scene->drawimage();
}

void CamMod::rand_disturb()
{
    scene->rand_disturb();
}
