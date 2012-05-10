#ifndef BASEANALYZER_H
#define BASEANALYZER_H

#include <QObject>

class BaseAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit BaseAnalyzer(QObject *parent = 0);
    virtual void DoAnalyze();
    virtual void showDialog();
    virtual void printResults(QWidget *);
    
signals:
    
public slots:
    
};

#endif // BASEANALYZER_H
