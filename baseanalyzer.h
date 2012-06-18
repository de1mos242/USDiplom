#ifndef BASEANALYZER_H
#define BASEANALYZER_H

#include <QObject>
#include <QTableWidget>

class BaseAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit BaseAnalyzer(QObject *parent = 0);
    QTableWidget* table;
    QList<QWidget *> AdditionalWidgets;
    virtual void DoAnalyze();
    virtual bool showDialog();
    virtual void printResults(QTableWidget *);
    QList <QString> *parametersList;
    void fillInputTableData();
    QString name;


signals:

public slots:

protected:
    QList <QString> headerList;
    virtual QHash<QString,QString> getAllParams();
    int columnsCount;
    int rowsCount;
    virtual void fillInputData();
    QHash<int,QList<double>* > *data;

    QList<QList <double> > multiMatrix(QList<QList <double> >,QList<QList <double> >);
    QList<QList <double> > transponate(QList<QList <double> >);
    QList<QList <double> > copyMatrix(QList<QList <double> >);

    double minElement(QList<double>);
    double maxElement(QList<double>);
    double average(QList<double>);
    double variance(QList<double>);
    double standardDeviation(QList<double>);

private:
    void fillHeaderList();

};

#endif // BASEANALYZER_H
