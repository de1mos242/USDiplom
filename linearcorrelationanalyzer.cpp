#include "linearcorrelationanalyzer.h"
#include "QTableWidgetItem"
#include <QTableWidget>
#include <QHeaderView>
#include <QGenericMatrix>
#include <qmath.h>
#include <QGridLayout>

LinearCorrelationAnalyzer::LinearCorrelationAnalyzer()
{
}

void LinearCorrelationAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    for (int i=0;i<matrix.count();i++) {
        QList<double> row;
        for(int j=0;j<matrix.count();j++) {
            row.append(findElement(matrix.at(i),matrix.at(j)));
        }
        results.append(row);
    }
}

double LinearCorrelationAnalyzer::findElement(QList<double> x, QList<double> y) {
    double avgX = average(x);
    double avgY = average(y);

    QList<double> xy;
    for (int i=0;i<x.count();i++)
        xy.append(x.at(i)*y.at(i));
    double avgXY = average(xy);

    double divX = standardDeviation(x);
    double divY = standardDeviation(y);

    return ((avgXY - avgX * avgY)/(divX * divY));
}

void LinearCorrelationAnalyzer::printResults(QTableWidget * table) {
    int parametersCount = parametersList->count();
    table->setColumnCount(parametersCount);
    table->setRowCount(parametersCount);
    for (int i=0;i<parametersCount;i++) {
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
        table->setVerticalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<parametersCount;i++)
        for(int j=0;j<parametersCount;j++)
            table->setItem(i,j,new QTableWidgetItem(QString::number(results[i][j])));
}

void LinearCorrelationAnalyzer::fillInputData() {
    QList<QList<double> > tempMatrix;
    bool skipRow = false;
    for (int row=0;row<rowsCount;row++) {
        QList<double> rowList;
        for (int col=0;col<parametersList->count();col++) {
            int tableColuem = parametersList->at(col).toInt();
            QString cell = table->item(row, tableColuem)->text();
            if (cell == "@") {
                skipRow = true;
                break;
            }
            rowList.append(cell.toDouble());
        }
        if (!skipRow)
            tempMatrix.append(rowList);
        else
            skipRow = false;
    }

    for(int col=0;col<tempMatrix.at(0).count();col++) {
        QList<double> colList;
        for (int row=0;row<tempMatrix.count();row++) {
            colList.append(tempMatrix.at(row).at(col));
        }
        matrix.append(colList);
    }
}

QHash<QString,QString> LinearCorrelationAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    for (int i=0;i<columnsCount;i++) {
        QString headerItemText = headerList.at(i);
        result.insert(QString::number(i), headerItemText);
    }
    return result;
}
