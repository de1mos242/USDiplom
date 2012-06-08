#ifndef TEXT3D_H
#define TEXT3D_H

#include "figure3d.h"

class Text3D : public Figure3D
{
public:
    Text3D();
    void setPoint(Point3D pt);
    void setText(QString text);

    virtual void draw();

private:
    Point3D center;
    QString text;
};

#endif // TEXT3D_H
