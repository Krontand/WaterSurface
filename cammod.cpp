#include "cammod.h"

CamMod::CamMod(QObject *parent) : QObject(parent)
{
    xpos = -1;
    ypos = -1;
}

void CamMod::mouse_move(QMouseEvent *pe)
{
    if (!moving)
        return;
    if (xpos == -1)
    {
        xpos = pe->x();
        ypos = pe->y();
        return;
    }
    if (xpos != pe->x())
        rotatex((pe->x() - xpos)/50.0);
    if (ypos != pe->y())
        rotatey((ypos - pe->y())/50.0);
    xpos = pe->x();
    ypos = pe->y();

    scene->cam->eye[2] = scene->cam->dist * sin(scene->cam->angley) * cos(scene->cam->anglex);
    scene->cam->eye[0] = scene->cam->dist * sin(scene->cam->angley) * sin(scene->cam->anglex);
    scene->cam->eye[1] = scene->cam->dist * cos(scene->cam->angley);
}

void CamMod::mouse_release(QMouseEvent *pe)
{
    xpos = -1;
    moving = false;
}

void CamMod::mouse_wheel(QWheelEvent *pe)
{
    float scale = pe->delta() / 1200.0;

    if (pe->orientation() == Qt::Vertical)
    {
        scene->scale(scale);
    }
    pe->accept();
}

void CamMod::setpower(int p)
{
    scene->set_disturb_power(p/1000.0);
}

void CamMod::reset_water()
{
    scene->reset_water();
}

void CamMod::mouse_press(QMouseEvent *pe)
{
    if (pe->button() == Qt::LeftButton)
        moving = true;
    if (pe->button() == Qt::RightButton)
        this->rightclick(pe);

}

void CamMod::rotatey(double angle)
{
    scene->cam->angley += angle;

    if (scene->cam->angley < 1E-2)
        scene->cam->angley = 1E-2;
    if (scene->cam->angley > M_PI)
        scene->cam->angley = M_PI;

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

void CamMod::leftclick(QMouseEvent *pe)
{

}

void CamMod::rightclick(QMouseEvent *pe)
{
    scene->disturb(pe->x(), pe->y());
}
