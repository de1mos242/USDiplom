#ifndef PAIRREGRESSIONANALYZER_H
#define PAIRREGRESSIONANALYZER_H
#include "baseanalyzer.h"
#include "figure3d.h"

class PairRegressionAnalyzer : public BaseAnalyzer
{
public:
    PairRegressionAnalyzer();
    virtual bool showDialog();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    double R;

    QList<Point3DEx> getYPoints();
    QList<Point3D> getY2Points();

    double A, B;
};

#endif // PAIRREGRESSIONANALYZER_H
