#ifndef ADAPTIVEGROUPANALYZEGRAPHIC_H
#define ADAPTIVEGROUPANALYZEGRAPHIC_H
#include "glwidget.h"


class AdaptiveGroupAnalyzeGraphic : public GLWidget
{
public:
    AdaptiveGroupAnalyzeGraphic(QWidget *parent = 0);
    ~AdaptiveGroupAnalyzeGraphic();
    void SetPoints(QList<QList<double> > points);

protected:
    virtual void drawGraphic();
    virtual void drawCoords();

private:
    QList<QList<double> > points;
    int pointsXLen;
    double pointsYMax;

    QList<float> rList;
    QList<float> gList;
    QList<float> bList;
};

#endif // ADAPTIVEGROUPANALYZEGRAPHIC_H

