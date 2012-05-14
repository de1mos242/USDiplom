#ifndef COMPONENTANALYZER_H
#define COMPONENTANALYZER_H

#include <QHash>
#include <QList>
#include "baseanalyzer.h"

class ComponentAnalyzer : public BaseAnalyzer
{
public:
    ComponentAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    double e1;
    double e2;

    QList<QList<double> > PMatrix;
    QList<QList<double> > TMatrix;
};

#endif // COMPONENTANALYZER_H
