#ifndef LINEARCORRELATIONANALYZER_H
#define LINEARCORRELATIONANALYZER_H

#include "baseanalyzer.h"
#include <QHash>
#include <QList>

class LinearCorrelationAnalyzer : public BaseAnalyzer
{
public:
    LinearCorrelationAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    double findElement(QList<double>,QList<double>);
};

#endif // LINEARCORRELATIONANALYZER_H
