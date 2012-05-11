#ifndef BASEANALYZER_H
#define BASEANALYZER_H

#include <QObject>
#include <QTableWidget>

class BaseAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit BaseAnalyzer(QObject *parent = 0);
    virtual void DoAnalyze(QTableWidget *);
    virtual bool showDialog();
    virtual void printResults(QTableWidget *);
    
signals:

public slots:
    
};

#endif // BASEANALYZER_H
