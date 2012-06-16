#ifndef LINE3D_H
#define LINE3D_H

#include "figure3d.h"

class Line3D : public Figure3D
{
public:
    Line3D();

    void setPoints(Point3D from, Point3D to);

    virtual void draw();

private:
    Point3D from;
    Point3D to;
};

#endif // LINE3D_H
