#ifndef STATISTICANALYZER_H
#define STATISTICANALYZER_H

#include "baseanalyzer.h"
#include <QHash>
#include <QList>

class StatisticAnalyzer : public BaseAnalyzer
{
public:
    StatisticAnalyzer();
    virtual void DoAnalyze(QTableWidget *);
    virtual bool showDialog();
    virtual void printResults(QTableWidget *);

private:
    QHash<QString,QString> getAllParams();
    QList <QString> *parametersList;

    QList <QString> headerList;
    int columnsCount;
    int rowsCount;
    QHash<int,QHash<QString, double>* > *results;
    QHash<int,QList<double>* > *data;

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
