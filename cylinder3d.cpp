#include "cylinder3d.h"
#include "GL/glu.h"
#include "math.h"
#include "QtOpenGL"

Cylinder3D::Cylinder3D()
{
}

void Cylinder3D::setGeometry(Point3D p1, Point3D p2, GLfloat r) {
    point1 = p1;
    point2 = p2;
    radius = r;
}

void Cylinder3D::draw() {
    Figure3D::draw();

    GLfloat height = sqrt((point1.x-point2.x)*(point1.x-point2.x)  +
                          (point1.y-point2.y)*(point1.y-point2.y) +
                          (point1.z-point2.z)*(point1.z-point2.z));

    /*Point3D center;
    center.x = (point1.x + point2.x)/2;
    center.y = (point1.y + point2.y)/2;
    center.z = (point1.z + point2.z)/2;*/

    float vx = point2.x-point1.x;
    float vy = point2.y-point1.y;
    float vz = point2.z-point1.z;

    //handle the degenerate case of z1 == z2 with an approximation
    if(vz == 0)
        vz = .0001;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;



    GLUquadricObj * quadratic=gluNewQuadric();
    glPushMatrix();
    glTranslatef(point1.x,point1.y,point1.z);
    glRotatef(ax, rx, ry, 0.0);
    gluCylinder(quadratic, radius, radius, height, 32,1);
    glPopMatrix();
}
