#include "baseanalyzer.h"

#include "statisticanalyzedialog.h"
#include "qmath.h"
#include <QGLWidget>

BaseAnalyzer::BaseAnalyzer(QObject *parent) :
    QObject(parent)
{

}

bool BaseAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    dialog->SetDialogName(name);
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
    QList<QList<double> > tempTable;
    for (int row=0;row<rowsCount;row++) {
        QList<double> tempRow;
        tempRow.clear();
        bool skip = false;
        for (int col=0;col<columnsCount;col++) {
            QString cell = table->item(row, col)->text();
            if (cell == "@")
                skip = true;
            tempRow.append(cell.toDouble());
        }
        if (!skip)
            tempTable.append(tempRow);
    }

    rowsCount = tempTable.size();

    data = new QHash<int,QList<double>* >();
    QList<double> *column;
    for (int col=0;col<columnsCount;col++) {
        column = new QList<double>();
        for (int row=0;row<rowsCount;row++) {
            column->append(tempTable[row][col]);
        }
        data->insert(col, column);
    }
}

QHash<QString,QString> BaseAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    for (int i=0;i<columnsCount;i++) {
        QString headerItemText = headerList.at(i);
        result.insert(QString::number(i), headerItemText);
    }
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

double BaseAnalyzer::average(QList<double> list) {
    double sum = 0;
    for (int i=0;i<list.count();i++)
        sum+=list.at(i);
    return (sum/list.count());
}

double BaseAnalyzer::variance(QList<double> list) {
    double avg = average(list);
    double sum = 0;
    for (int i=0;i<list.count();i++) {
        double temp =(list.at(i) - avg);
        sum += temp*temp;
    }
    return sum/list.count();
}

double BaseAnalyzer::standardDeviation(QList<double> list) {
    double vrc = variance(list);
    return sqrt(vrc);
}

double BaseAnalyzer::minElement(QList<double> list) {
    double min = list.at(0);
    for (int i=1;i<list.count();i++)
        if (list.at(i) < min)
            min = list.at(i);
    return (min);
}

double BaseAnalyzer::maxElement(QList<double> list) {
    double max = list.at(0);
    for (int i=1;i<list.count();i++)
        if (list.at(i) > max)
            max = list.at(i);
    return (max);
}

double BaseAnalyzer::sumElements(QList<double> list) {
    double temp = 0.0;
    for (int i=0;i<list.size();i++)
        temp += list[i];
    return temp;
}

double BaseAnalyzer::sumListsElements(QList<double> list1, QList<double> list2) {
    double temp = 0.0;
    for (int i=0;i<list1.size(); i++)
        temp += list1[i]*list2[i];
    return temp;
}

QList<QList<double> > BaseAnalyzer::createNullMatrix(int n) {
    QList<QList<double > > result;
    for (int i=0;i<n;i++) {
        result.append(createNullList(n));
    }
    return result;
}

QList<double> BaseAnalyzer::createNullList(int n) {
    QList<double> result;
    for (int i=0;i<n;i++)
        result.append(0.0);
    return result;
}
