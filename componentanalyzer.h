#ifndef COMPONENTANALYZER_H
#define COMPONENTANALYZER_H

#include <QHash>
#include <QList>
#include "baseanalyzer.h"
#include "graphnode.h"
#include "figure3d.h"

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
    QList<QList<double> > iMatrix;
    QList<QList<double> > iTMatrix;

    void getIMatrix();
    void getITmatrix();
    double getCloudCenterByCoord(double one, double two);
    Point3D getPointInCloud(QList<QList<double> > cloud);
    QList<QList<double> > getMaxCloudFromSectors(QList<QList<QList<double> > > sectors);

    double e1;
    double e2;

    QList<QList<double> > PMatrix;
    QList<QList<double> > TMatrix;

    void normalize();

    QList<QList<QPair<QString, double> > > prepareGraphicData();
    GraphNode* prepareGraphData();

    QList<QList<QList<double> > > getSectors(QList<QList<double> > cloud);
    Point3D getCloudCenter(QList<QList<double> > cloud);

    QList<Point3D> graphNodePoints;
};

#endif // COMPONENTANALYZER_H
