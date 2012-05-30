#ifndef SPEARMANANALIZER_H
#define SPEARMANANALIZER_H

#include "baseanalyzer.h"

class SpearmanAnalizer : public BaseAnalyzer
{
public:
    SpearmanAnalizer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    double findElement(QList<double>,QList<double>);
    QList<double> calcRang(QList<double>, double&);
};

#endif // SPEARMANANALIZER_H
