#ifndef ELACTICMAPANALYZER_H
#define ELACTICMAPANALYZER_H
#include "baseanalyzer.h"
#include "figure3d.h"

class ElacticMapAnalyzer : public BaseAnalyzer
{
public:
    ElacticMapAnalyzer();
    virtual bool showDialog();
    virtual void DoAnalyze();
    virtual void printResults(QTableWidget *);

protected:
    virtual void fillInputData();
    virtual QHash<QString,QString> getAllParams();

private:
    int p,q, pq;
    double mu, lambda;
    QList<QList<double> > TMatrix;

    QList<Point3D> inputPoints;
    void fillInputPoints();

    QList<QList<double> > AMatrix;
    void fillAMatrix();
    double findAElement(int type, int k, int l);

    int kron(int a, int b);

    QList<int> taxonsCount;
    QList<QList<int> > prevTaxonsCountHistory;
    QList<int> getTaxonsCount();
    QList<Point3D> findTaxonPoints(int idx);

    QList<Point3D> taxons;
    void initTaxons();


    QList<Point3D> BMatrix;
    QList<Point3D> BMatrixOld;
    void fillBMatrix();

    void findNewTaxons();
    bool compareTaxonsCount();

    QList<QList<double> > findTriangleAMatrix();

    QList<Point3DEx> getTmatrix();
    QList<QList<Point3D> > getNet();

    int iterations;

};

#endif // ELACTICMAPANALYZER_H
