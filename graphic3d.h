#ifndef GRAPHIC3D_H
#define GRAPHIC3D_H
#include "glwidget.h"

class Graphic3d : public GLWidget
{
public:
    Graphic3d(QWidget *parent = 0);
    ~Graphic3d();
    void SetPoints(QList<Point3DEx> points);
    void SetNet(QList<QList<Point3D> > nodes);

protected:
    virtual void drawGraphic();

private:
    QList<Point3DEx> points;
    QList<QList<Point3D> > netNodes;
};

#endif // GRAPHIC3D_H
