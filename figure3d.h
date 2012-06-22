#ifndef FIGURE3D_H
#define FIGURE3D_H

#include <QtOpenGL>

struct FigureColor {
    float r,g,b;
};


class Figure3D
{
public:
    Figure3D();


    enum GColor {
        black, white, red, green, blue, yellow,darkGreen,fromRGB
    };

    virtual void draw();
    void SetCurrentColor(GColor);
    void SetCurrentColor(FigureColor color);
    void SetCurrentColor(float r, float g, float b);
    static void SetNewColor(GColor);
    static void SetNewColor(float r, float g, float b);

protected:
    static void setColor(FigureColor c);

    FigureColor currentColor;
    static FigureColor colorFromGColor(GColor c);
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
