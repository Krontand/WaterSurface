#ifndef CAMMOD_H
#define CAMMOD_H


#include <QObject>
#include <QtGui>
#include "scene.h"

class CamMod : public QObject
{
    Q_OBJECT
public:
    explicit CamMod(QObject *parent = 0);
    Scene *scene;
    void mouseEvent(QMouseEvent *pe);

    void mouse_move(QMouseEvent *pe);
    void mouse_press(QMouseEvent *pe);
    void mouse_release(QMouseEvent *pe);
    void mouse_wheel(QWheelEvent *pe);

    void setpower(int p);
    void reset_water();

public slots:
    void updatescene();
    void rand_disturb();

private:
    void leftclick(QMouseEvent *pe);
    void rightclick(QMouseEvent *pe);

    void rotatey(double angle);
    void rotatex(double angle);
    int xpos;
    int ypos;
    void drawimage();
    bool moving;
};

#endif // CAMMOD_H
