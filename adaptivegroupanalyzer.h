#ifndef ADAPTIVEGROUPANALYZER_H
#define ADAPTIVEGROUPANALYZER_H
#include "baseanalyzer.h"


class AdaptiveGroupAnalyzer : public BaseAnalyzer
{
public:
    AdaptiveGroupAnalyzer();
    virtual bool showDialog();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    QList<QList<QString> > subSystemsParams;
    QList<QList<QList<double> > > subSystemsData;

    QList<QList<QList<double> > > componentMatricies;

};

#endif // ADAPTIVEGROUPANALYZER_H
