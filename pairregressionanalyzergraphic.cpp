#include "pairregressionanalyzergraphic.h"
#include "line3d.h"

PairRegressionAnalyzerGraphic::PairRegressionAnalyzerGraphic(QWidget *parent)
    : Graphic2D(parent)
{
    dimentions = 2;
}

PairRegressionAnalyzerGraphic::~PairRegressionAnalyzerGraphic()
{
}

void PairRegressionAnalyzerGraphic::drawGraphic() {
    drawPoints();
    drawLine();
    drawError();
}

void PairRegressionAnalyzerGraphic::SetLinePoints(QList<Point3D> points) {
    this->linePoints = points;
}

void PairRegressionAnalyzerGraphic::SetErrorText(QString error) {
    this->error = error;
}

void PairRegressionAnalyzerGraphic::drawLine() {
    Line3D line;
    line.SetCurrentColor(Line3D::black);
    for (int i=1;i<linePoints.size();i++) {
        line.setPoints(linePoints[i-1], linePoints[i]);
        line.draw();
    }
}

void PairRegressionAnalyzerGraphic::drawError() {
    QFont* font = new QFont("Times", 14, QFont::Normal);
    renderText(-coords.x, coords.y*0.75, 0.0, error, *font);
}
