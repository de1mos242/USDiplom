#include "figure3d.h"

Figure3D::Figure3D()
{
}

void Figure3D::SetCurrentColor(GColor color) {
    currentColor = color;
}

void Figure3D::setColor(GColor color) {
    switch (color) {
    case black:
        glColor3f(0.0f,0.0f,0.0f);
        break;
    case red:
        glColor3f(1.0f,0.0f,0.0f);
        break;
    case green:
        glColor3f(0.0f,1.0f,0.0f);
        break;
    case blue:
        glColor3f(0.0f,0.0f,1.0f);
        break;
    case white:
        glColor3f(1.0f,1.0f,1.0f);
        break;
    case yellow:
        glColor3f(1.0f,1.0f,0.0f);
        break;
    case darkGreen:
        glColor3f(0.0f,0.3f,0.0f);
        break;
    }
}

void Figure3D::SetNewColor(GColor color) {
    setColor(color);
}

void Figure3D::draw() {
    setColor(currentColor);
}
