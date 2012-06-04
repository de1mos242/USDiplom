#ifndef CYLINDER3D_H
#define CYLINDER3D_H

#include "figure3d.h"

class Cylinder3D : public Figure3D
{
public:
    Cylinder3D();

    virtual void draw();
    void setGeometry(Point3D p1, Point3D p2, GLfloat r);

private:
    Point3D point1;
    Point3D point2;
    GLfloat radius;
};

#endif // CYLINDER3D_H
