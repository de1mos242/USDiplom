#include "vector.h"
#include "math.h"

Vector::Vector()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector Vector::normalize() {
    Vector nv;
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return nv;
    double curLen = length();
    nv.x = x/curLen;
    nv.y = y/curLen;
    nv.z = z/curLen;
    return nv;
}

double Vector::length() {
    return sqrt(x*x+y*y+z*z);
}

Vector Vector::Create(Point3D from, Point3D to) {
    Vector nv;
    nv.x = to.x - from.x;
    nv.y = to.y - from.y;
    nv.z = to.z - from.z;
    return nv;
}

Point3D Vector::movePoint(Point3D from, double length) {
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return from;
    Vector normal = normalize();
    normal.x*= length;
    normal.y*= length;
    normal.z*= length;

    Point3D np;
    np.x = from.x + normal.x;
    np.y = from.y + normal.y;
    np.z = from.z + normal.z;
    return np;
}

Vector Vector::sum(Vector add) {
    Vector nv;
    nv.x = x + add.x;
    nv.y = y + add.y;
    nv.z = z + add.z;
    return nv;
}

