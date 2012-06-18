#include "graphic2d.h"
#include "snowflake3d.h"

Graphic2D::Graphic2D(QWidget *parent)
    : GLWidget(parent)
{
    dimentions = 2;
}

Graphic2D::~Graphic2D()
{
}

void Graphic2D::drawGraphic() {
    drawPoints();
}

void Graphic2D::drawPoints() {
    SnowFlake3D flake;
    QFont* font = new QFont("Times", 14, QFont::Normal);
    foreach (Point3DEx p, points) {
        flake.SetCurrentColor(p.color);
        flake.SetGeometry(p.getPoint3D(), 0.04f);
        flake.draw();
        Figure3D::SetNewColor(Figure3D::darkGreen);
        renderText(p.x, p.y, p.z, p.label, *font);
    }
}

void Graphic2D::SetPoints(QList<Point3DEx> points) {
    this->points = points;
}
