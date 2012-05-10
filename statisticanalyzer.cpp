#include "statisticanalyzer.h"
#include "baseanalyzer.h"
#include <QMessageBox>
#include <QDialog>
#include "statisticanalyzedialog.h"

StatisticAnalyzer::StatisticAnalyzer():BaseAnalyzer()
{
}
void BaseAnalyzer::DoAnalyze() {
    showDialog();
}

void BaseAnalyzer::showDialog() {
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    dialog->exec();
}

void BaseAnalyzer::printResults(QWidget * tab) {

}
