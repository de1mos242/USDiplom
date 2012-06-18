#ifndef GRAPHIC2D_H
#define GRAPHIC2D_H
#include "glwidget.h"

class Graphic2D : public GLWidget
{
public:
    Graphic2D(QWidget *parent = 0);
    ~Graphic2D();
    void SetPoints(QList<Point3DEx> points);

protected:
    virtual void drawGraphic();
    virtual void drawPoints();

private:
    QList<Point3DEx> points;
};

#endif // GRAPHIC2D_H
