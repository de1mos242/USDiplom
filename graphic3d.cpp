#include "graphic3d.h"
#include "snowflake3d.h"
#include "line3d.h"

Graphic3d::Graphic3d(QWidget *parent)
    : GLWidget(parent)
{
    dimentions = 3;
}

Graphic3d::~Graphic3d()
{
}

void Graphic3d::drawGraphic() {
    SnowFlake3D flake;
    QFont* font = new QFont("Times", 14, QFont::Normal);
    foreach (Point3DEx p, points) {
        flake.SetCurrentColor(p.color);
        flake.SetGeometry(p.getPoint3D(), 0.04f);
        flake.draw();
        Figure3D::SetNewColor(Figure3D::darkGreen);
        renderText(p.x, p.y, p.z, p.label, *font);
    }

    Line3D line;
    line.SetCurrentColor(Line3D::black);
    for (int i=0;i<netNodes.size();i++) {
        for (int j=0;j<netNodes[i].size();j++) {
            if (i>0) {
                line.setPoints(netNodes[i-1][j], netNodes[i][j]);
                line.draw();
            }
            if (j>0) {
                line.setPoints(netNodes[i][j-1], netNodes[i][j]);
                line.draw();
            }
        }
    }
}

void Graphic3d::SetPoints(QList<Point3DEx> points) {
    this->points = points;
}

void Graphic3d::SetNet(QList<QList<Point3D> > nodes) {
    netNodes = nodes;
}
