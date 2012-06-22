#include "adaptivegroupanalyzegraphic.h"
#include "snowflake3d.h"
#include "line3d.h"
#include "math.h"

AdaptiveGroupAnalyzeGraphic::AdaptiveGroupAnalyzeGraphic(QWidget *parent)
    : GLWidget(parent)
{
    dimentions = 2;
}

AdaptiveGroupAnalyzeGraphic::~AdaptiveGroupAnalyzeGraphic()
{
}

void AdaptiveGroupAnalyzeGraphic::drawGraphic() {
    SnowFlake3D flake;
    QFont* font = new QFont("Times", 12, QFont::Normal);
    for (int i=0;i<points.size();i++) {
        flake.SetCurrentColor(rList[i],gList[i],bList[i]);
        for (int j=0;j<points[i].size();j++) {
            Point3D p;
            p.x = (j+1)/coordsScale.x;
            p.y = fabs(points[i][j]/coordsScale.y);
            flake.SetGeometry(p, 0.04f);
            flake.draw();
        }
        Figure3D::SetNewColor(rList[i],gList[i],bList[i]);
        renderText(-coords.x, coords.y - 1 - i*0.5f,0.0f, "Подсистема " + QString::number(i+1), *font);
    }
}

void AdaptiveGroupAnalyzeGraphic::SetPoints(QList<QList<double> > points) {
    this->points = points;
    pointsXLen = points[0].size() + 1;
    coordsScale.x = pointsXLen/coords.x;

    pointsYMax = fabs(points[0][0]);
    for (int i=0;i<points.size();i++) {
        for (int j=0;j<points[i].size();j++) {
            if (fabs(points[i][j]) > pointsYMax)
                pointsYMax = fabs(points[i][j]);
        }
    }

    coordsScale.y = pointsYMax/coords.y;

    for (int i=0;i<points.size();i++) {
        rList.append(0.5f + i*0.5f/(points[0].size()-1));
        gList.append(1.0f - i*0.5f/(points[0].size()-1));
        bList.append(0.5f - i*0.5f/(points[0].size()-1));
    }
}

void AdaptiveGroupAnalyzeGraphic::drawCoords() {
    QFont* font = new QFont("Times", 14, QFont::Normal);
    Line3D xCoord;
    xCoord.SetCurrentColor(coordsColor);
    GLfloat xLen = coords.x;
    Point3D xFrom;
    xFrom.x = -xLen;
    Point3D xTo;
    xTo.x = xLen;
    xCoord.setPoints(xFrom, xTo);
    xCoord.draw();
    Figure3D::SetNewColor(coordsLabelsColor);
    renderText(xLen, 0.0f,0.0f,coordsLabels[0], *font);

    Line3D yCoord;
    yCoord.SetCurrentColor(coordsColor);
    GLfloat yLen = coords.y;
    Point3D yFrom;
    yFrom.y = -yLen;
    Point3D yTo;
    yTo.y = yLen;
    yCoord.setPoints(yFrom, yTo);
    yCoord.draw();
    Figure3D::SetNewColor(coordsLabelsColor);
    renderText(0.0f,yLen, 0.0f,coordsLabels[1], *font);

    GLfloat tickSize = 0.04f;
    SnowFlake3D tick;
    tick.SetCurrentColor(coordsColor);
    for (int i=(int)-pointsXLen+1;i<pointsXLen;i++) {
        Point3D tickCenter;
        tickCenter.x = i/coordsScale.x;
        tick.SetGeometry(tickCenter, tickSize);
        tick.draw();
        renderText(i/coordsScale.x, 0.0f,0.0f, QString::number(i), *font);
    }
    for (int i=(int)-yLen;i<(int)yLen+1;i++) {
        Point3D tickCenter;
        tickCenter.y = i;
        tick.SetGeometry(tickCenter, tickSize);
        tick.draw();
        renderText(0.0f,i, 0.0f, QString::number((i*coordsScale.y)), *font);
    }
}

