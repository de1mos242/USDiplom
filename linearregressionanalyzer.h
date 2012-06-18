#ifndef LINEARREGRESSIONANALYZER_H
#define LINEARREGRESSIONANALYZER_H
#include "baseanalyzer.h"

class LinearRegressionAnalyzer : public BaseAnalyzer
{
public:
    LinearRegressionAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<double> results;
    QList<QList<double> > matrix;

    double R2;

    QList<double> Gauss(QList<QList<double> > a, QList<double> y);

};

#endif // LINEARREGRESSIONANALYZER_H
