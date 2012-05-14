#include "analyzerstrategy.h"

AnalyzerStrategy::AnalyzerStrategy(QObject *parent) :
    QObject(parent)
{
}

bool AnalyzerStrategy::Run() {
    analyzer->table = sourcetable;
    if (!analyzer->showDialog())
        return false;

    analyzer->DoAnalyze();
    analyzer->printResults(resultTable);
    return true;
}
