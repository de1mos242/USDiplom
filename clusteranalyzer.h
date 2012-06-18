#ifndef CLUSTERANALYZER_H
#define CLUSTERANALYZER_H

#include "baseanalyzer.h"

class ClusterAnalyzer : public BaseAnalyzer
{
public:
    ClusterAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);
    virtual bool showDialog();

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<QList<int> > results;
    QList<QList<double> > matrix;

    QList<QList<double> > clusters;
    QList<QList<int> > clustersFill;
    QList<QList<int> > prevClustersFill;
    bool checkClustersCounts();

    int ClustersNumber;

    double getDistance(QList<double> from, QList<double> to);
    QList<QList<double> > findNewClusters();
    void fillClusters();

    bool checkHasSameCluster(QList<double> cluster);
};

#endif // CLUSTERANALYZER_H
