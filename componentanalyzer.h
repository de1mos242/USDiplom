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
    bool ShowGraph;

protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();
    virtual bool showDialog();

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

    QList<QList<QList<double> > > getSectors(QList<QList<double> > cloud, int divide);
    Point3D getCloudCenter(QList<QList<double> > cloud);

    QList<QPair<Point3D,double> > graphNodePoints;

    void getCellPoints(QList<QList<double> > cloud, int divide);

    void shiftPoints();
    void mergePoints();

    QList<Point3D> cloud;
    void convertCould(QList<QList<double> > cloud);
    void normalizeRaduises();

    int iterationCount;
    double shiftK;
    double mergeK;
    int divideStep;
};

#endif // COMPONENTANALYZER_H
