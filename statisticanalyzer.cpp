#include "statisticanalyzer.h"
#include "baseanalyzer.h"
#include <QMessageBox>

StatisticAnalyzer::StatisticAnalyzer():BaseAnalyzer()
{
}
void BaseAnalyzer::DoAnalyze() {
    QMessageBox msgBox;
    msgBox.setInformativeText("I do statistic analyze");
    msgBox.setWindowTitle("Analyzer say's:");
    msgBox.exec();
}

