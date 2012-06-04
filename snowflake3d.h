#ifndef SNOWFLAKE3D_H
#define SNOWFLAKE3D_H

#include "figure3d.h"

class SnowFlake3D : public Figure3D
{
public:
    SnowFlake3D();

    virtual void draw();

    void SetGeometry(Point3D center, GLfloat size);

private:
    Point3D center;
    GLfloat size;
};

#endif // SNOWFLAKE3D_H
