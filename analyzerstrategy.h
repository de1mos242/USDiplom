#ifndef ANALYZERSTRATEGY_H
#define ANALYZERSTRATEGY_H

#include <QObject>
#include "baseanalyzer.h"

class AnalyzerStrategy : public QObject
{
    Q_OBJECT
public:
    explicit AnalyzerStrategy(QObject *parent = 0);
    BaseAnalyzer *analyzer;
    QTableWidget *sourcetable;
    QTableWidget *resultTable;

    bool Run();
signals:
    
public slots:
    
};

#endif // ANALYZERSTRATEGY_H
