#include "baseanalyzer.h"

BaseAnalyzer::BaseAnalyzer(QObject *parent) :
    QObject(parent)
{
}

bool BaseAnalyzer::showDialog() {
    return false;
}

void BaseAnalyzer::DoAnalyze(QTableWidget *) {

}

void BaseAnalyzer::printResults(QTableWidget *) {

}
