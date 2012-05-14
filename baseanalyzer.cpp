#include "baseanalyzer.h"

#include "statisticanalyzedialog.h"

BaseAnalyzer::BaseAnalyzer(QObject *parent) :
    QObject(parent)
{

}

bool BaseAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    QHashIterator<QString,QString> iterator(this->getAllParams());
    while (iterator.hasNext()) {
        iterator.next();
        dialog->addAviabledParam(iterator.value(), iterator.key());
    }
    dialog->exec();
    if (dialog->ParametersList->count()==0) {
        return false;
    }
    parametersList = dialog->ParametersList;
    return true;
}

void BaseAnalyzer::DoAnalyze() {
    fillInputData();
}

void BaseAnalyzer::printResults(QTableWidget *) {

}

void BaseAnalyzer::fillInputData() {
    data = new QHash<int,QList<double>* >();
    QList<double> *column;
    for (int col=0;col<columnsCount;col++) {
        column = new QList<double>();
        for (int row=0;row<rowsCount;row++) {
            QString cell = table->item(row, col)->text();
            if (cell != "@")
                column->append(cell.toDouble());
        }
        data->insert(col, column);
    }
}

QHash<QString,QString> BaseAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    return result;
}

void BaseAnalyzer::fillHeaderList(){
    for (int i=0;i<columnsCount;i++)
        headerList.append(table->horizontalHeaderItem(i)->text());
}

void BaseAnalyzer::fillInputTableData() {
    columnsCount = table->columnCount();
    rowsCount = table->rowCount();
    fillHeaderList();
}

QList<QList <double> > BaseAnalyzer::multiMatrix(QList<QList<double> > first, QList<QList<double> > second) {
    QList<QList <double> > result;
    for(int row1 = 0;row1<first.count();row1++) {
        QList<double> row;
        for (int col2 = 0;col2<second[0].count();col2++) {
            double sum = 0;
            for (int i=0;i<second.count();i++)
                sum += first[row1][i]*second[i][col2];
            row.append(sum);
        }
        result.append(row);
    }
    return result;
}

QList<QList <double> > BaseAnalyzer::transponate(QList<QList <double> > mx) {
    QList<QList <double> > result;
    for (int col=0;col<mx[0].count();col++) {
        QList<double> temp;
        for (int row=0;row<mx.count();row++) {
            temp.append(mx[row][col]);
        }
        result.append(temp);
    }
    return result;
}

QList<QList <double> > BaseAnalyzer::copyMatrix(QList<QList <double> > mx) {
    QList<QList <double> > result;
    for (int row=0;row<mx.count();row++) {
        QList<double> temp;
        for (int col=0;col<mx[0].count();col++) {
            temp.append(mx[row][col]);
        }
        result.append(temp);
    }
    return result;
}
