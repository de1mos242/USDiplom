#ifndef STATISTICANALYZER_H
#define STATISTICANALYZER_H

#include "baseanalyzer.h"
#include <QHash>
#include <QList>

class StatisticAnalyzer : public BaseAnalyzer
{
public:
    StatisticAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual QHash<QString,QString> getAllParams();

private:

    QHash<int,QHash<QString, double>* > *results;

    void calcColumn(int);

    double calcMin(int);
    double calcMax(int);
    double calcAverage(int);
    double calcVariance(int);
    double calcStandardDeviation(int);
    double calcMedian(int);
    double calcQuartile25(int);
    double calcQuartile75(int);
    double calcMode(int);
    double calcAsymmetry(int);
    double calcExcess(int);

    QList<double>* sortArray(QList<double>*);
};

#endif // STATISTICANALYZER_H
