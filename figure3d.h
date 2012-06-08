#ifndef FIGURE3D_H
#define FIGURE3D_H

#include <QtOpenGL>

struct Point3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class Figure3D
{
public:
    Figure3D();

    enum GColor {
        black, white, red, green, blue, yellow
    };

    virtual void draw();
    void SetCurrentColor(GColor);
    static void SetNewColor(GColor);

protected:
    static void setColor(GColor);

    GColor currentColor;
};

#endif // FIGURE3D_H
