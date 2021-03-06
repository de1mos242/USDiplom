#ifndef PAIRREGRESSIONANALYZERGRAPHIC_H
#define PAIRREGRESSIONANALYZERGRAPHIC_H
#include "graphic2d.h"

class PairRegressionAnalyzerGraphic : public Graphic2D
{
public:
    PairRegressionAnalyzerGraphic(QWidget *parent = 0);
    ~PairRegressionAnalyzerGraphic();
    void SetLinePoints(QList<Point3D> points);
    void SetErrorText(QString error);
    void SetEquation(QString qu);

protected:
    virtual void drawGraphic();


private:
    void drawLine();
    void drawError();
    QList<Point3D> linePoints;
    QString error;
    QString equation;
};

#endif // PAIRREGRESSIONANALYZERGRAPHIC_H
