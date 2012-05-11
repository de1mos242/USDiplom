#include "analyzerstrategy.h"

AnalyzerStrategy::AnalyzerStrategy(QObject *parent) :
    QObject(parent)
{
}

bool AnalyzerStrategy::Run() {
    if (!analyzer->showDialog())
        return false;

    analyzer->DoAnalyze(sourcetable);
    analyzer->printResults(resultTable);
    return true;
}
