#include "snowflake3d.h"
#include "line3d.h"

SnowFlake3D::SnowFlake3D()
{
}

void SnowFlake3D::SetGeometry(Point3D center, GLfloat size) {
    this->center = center;
    this->size = size;
}

void SnowFlake3D::draw() {
    Figure3D::draw();

    Line3D lx, ly,lz;
    lx.SetCurrentColor(this->currentColor);
    Point3D lxs = center;
    lxs.x -= size;
    Point3D lxf = center;
    lxf.x += size;
    lx.setPoints(lxs, lxf);
    lx.draw();

    ly.SetCurrentColor(this->currentColor);
    Point3D lys = center;
    lys.y -= size;
    Point3D lyf = center;
    lyf.y += size;
    ly.setPoints(lys, lyf);
    ly.draw();

    lz.SetCurrentColor(this->currentColor);
    Point3D lzs = center;
    lzs.z -= size;
    Point3D lzf = center;
    lzf.z += size;
    lz.setPoints(lzs, lzf);
    lz.draw();

}
