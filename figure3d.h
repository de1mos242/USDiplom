#ifndef FIGURE3D_H
#define FIGURE3D_H

#include <QtOpenGL>


class Figure3D
{
public:
    Figure3D();

    enum GColor {
        black, white, red, green, blue, yellow,darkGreen
    };

    virtual void draw();
    void SetCurrentColor(GColor);
    static void SetNewColor(GColor);

protected:
    static void setColor(GColor);

    GColor currentColor;
};

struct Point3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    Point3D() : x(0.0f), y(0.0f),z(0.0f) {}
};

struct Point3DEx {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    QString label;
    Figure3D::GColor color;
    Point3DEx() : x(0.0f), y(0.0f),z(0.0f),label(""),color(Figure3D::black)   {}
    Point3D getPoint3D() {
        Point3D p;
        p.x = this->x;
        p.y = this->y;
        p.z = this->z;
        return p;
    }
};


#endif // FIGURE3D_H
