#include "line3d.h"

Line3D::Line3D()
{
}

void Line3D::setPoints(Point3D from, Point3D to) {
    this->from = from;
    this->to = to;
}

void Line3D::draw() {
    Figure3D::draw();
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex3f(from.x,from.y,from.z);
        glVertex3f(to.x,to.y,to.z);
    glEnd();
}
