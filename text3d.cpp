#include "text3d.h"
#include <QtGlobal>



Text3D::Text3D()
{
}

void Text3D::setPoint(Point3D pt) {
    center = pt;
}

void Text3D::setText(QString text) {
    this->text = text;
}

void Text3D::draw() {
    Figure3D::draw();
    glPushMatrix();

    glTranslatef(center.x,center.y,center.z);

    /*int length = text.length();
    for (i = 0; i < length; i++)
    {
        glColor3f((UpwardsScrollVelocity/10)+300+(l*10),(UpwardsScrollVelocity/10)+300+(l*10),0.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, quote[l][i]);
    }*/

    glPopMatrix();
}
