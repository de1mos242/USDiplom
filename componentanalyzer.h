#ifndef COMPONENTANALYZER_H
#define COMPONENTANALYZER_H

#include <QHash>
#include <QList>
#include "baseanalyzer.h"
#include "figure3d.h"

class ComponentAnalyzer : public BaseAnalyzer
{
public:
    ComponentAnalyzer();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);
    QList<QList<double> > TMatrix;


protected:
    virtual QHash<QString,QString> getAllParams();
    virtual void fillInputData();

private:
    QList<QList<double> > results;
    QList<QList<double> > matrix;

    double e1;
    double e2;

    QList<QList<double> > PMatrix;

    void normalize();

    QList<QList<double> > iMatrix;
    void getIMatrix();

    QList<Point3DEx> getTmatrix1();
    QList<Point3DEx> getTmatrix2();
    QList<Point3DEx> getPmatrix1();
    QList<Point3DEx> getPmatrix2();
};

#endif // COMPONENTANALYZER_H
