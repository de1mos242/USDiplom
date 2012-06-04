#ifndef SPHERE3D_H
#define SPHERE3D_H

#include "figure3d.h"

class Sphere3D : public Figure3D
{
public:
    Sphere3D();

    virtual void draw();

    void SetGeometry(Point3D center, GLfloat r);

private:
    Point3D center;
    GLfloat radius;
};

#endif // SPHERE3D_H
