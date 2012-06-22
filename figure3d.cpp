#include "figure3d.h"

Figure3D::Figure3D()
{
}

void Figure3D::SetCurrentColor(GColor color) {
    currentColor = colorFromGColor(color);
}

void Figure3D::SetCurrentColor(FigureColor color) {
    currentColor = color;
}

void Figure3D::SetCurrentColor(float r, float g, float b) {
    FigureColor res = {r,g,b};
    currentColor = res;
}

void Figure3D::setColor(FigureColor c) {
    glColor3f(c.r,c.g,c.b);
}

FigureColor Figure3D::colorFromGColor(GColor c) {
    FigureColor res = {0,0,0};
    switch (c) {
        case black:
            res.r = 0; res.b = 0; res.g = 0;
            break;
        case red:
            res.r = 1.0; res.g = 0; res.b = 0;
            break;
        case green:
            res.r = 0.0; res.g = 1.0; res.b = 0;
            break;
        case blue:
            res.r = 0.0; res.g = 0; res.b = 1.0;
            break;
        case white:
            res.r = 1.0; res.g = 1.0; res.b = 1.0;
            break;
        case yellow:
            res.r = 1.0; res.g = 1.0; res.b = 0;
            break;
        case darkGreen:
            res.r = 0.0; res.g = 0.3; res.b = 0;
            break;
        default:
            res.r = 0; res.b = 0; res.g = 0;
            break;
       }
    return res;
}

void Figure3D::SetNewColor(GColor c) {
    setColor(colorFromGColor(c));
}

void Figure3D::SetNewColor(float r, float g, float b) {
    FigureColor res = {r,g,b};
    setColor(res);
}

void Figure3D::draw() {
    setColor(currentColor);
}
