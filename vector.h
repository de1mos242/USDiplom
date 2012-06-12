#ifndef VECTOR_H
#define VECTOR_H
#include "figure3d.h"

class Vector
{
public:
    Vector();

    double x;
    double y;
    double z;

    Vector normalize();
    static Vector Create(Point3D from, Point3D to);
    double length();
    Point3D movePoint(Point3D from, double length);

    Vector sum(Vector add);
};

#endif // VECTOR_H
