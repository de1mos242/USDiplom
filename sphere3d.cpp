#include "sphere3d.h"
#include "GL/glu.h"
#include "QtOpenGL"

Sphere3D::Sphere3D()
{
}

void Sphere3D::SetGeometry(Point3D center, GLfloat r) {
    this->center = center;
    this->radius = r;
}

void Sphere3D::draw() {
    Figure3D::draw();

    GLUquadricObj * quadratic=gluNewQuadric();
    glPushMatrix();
    glTranslatef(center.x,center.y,center.z);
    gluSphere(quadratic, radius, 32,32);
    glPopMatrix();
}
