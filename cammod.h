#ifndef CAMMOD_H
#define CAMMOD_H


#include <QObject>
#include <QtGui>
#include "painter.h"

class CamMod : public QObject
{
    Q_OBJECT
public:
    explicit CamMod(QObject *parent = 0);
    Scene *scene;
    void mouseEvent(QMouseEvent *pe);

public slots:
    void updatescene();
    void rand_disturb();

private:
    void rotatey(double angle);
    void rotatex(double angle);
    int xpos;
    int ypos;
    void drawimage();
};

#endif // CAMMOD_H
