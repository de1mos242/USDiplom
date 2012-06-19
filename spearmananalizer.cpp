#include "spearmananalizer.h"
#include "QTableWidgetItem"
#include <QTableWidget>
#include <QHeaderView>
#include <QGenericMatrix>
#include <qmath.h>
#include <QGridLayout>

SpearmanAnalizer::SpearmanAnalizer()
{
}

void SpearmanAnalizer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    for (int i=0;i<matrix.count();i++) {
        QList<double> row;
        for(int j=0;j<matrix.count();j++) {
            row.append(findElement(matrix.at(i),matrix.at(j)));
        }
        results.append(row);
    }
}

double SpearmanAnalizer::findElement(QList<double> x, QList<double> y) {
    double Ta = 0;
    double Tb = 0;

    QList<double> rangX = calcRang(x, Ta);
    QList<double> rangY = calcRang(y, Tb);

    double delta = 0;
    for (int i=0;i<rangX.count();i++) {
        delta += (rangX[i] - rangY[i])*(rangX[i] - rangY[i]);
    }

    return (1 - 6*((delta+Ta+Tb)/(rangX.count()*(rangX.count()*rangX.count()-1))));
}

QList<double> SpearmanAnalizer::calcRang(QList<double> x, double &t) {
    QList<double> results;
    for (int i=0;i<x.size();i++)
        results.append(0);

    double globalMin = minElement(x);
    double globalMax = maxElement(x);
    t = 0;

    int rang = 0;

    QList<int> sames;
    double min;
    double lastMin = globalMin-1;
    while (lastMin < globalMax)
    {
        min = globalMax;
        for (int i=0;i<x.count();i++) {
            if (x[i] <= lastMin)
                continue;

            if (x[i] < min) {
                min = x[i];
                sames.clear();
            }

            if (x[i] == min) {
                sames.append(i);
            }
        }
        lastMin = min;


        if (sames.count() > 1) {
            int value = sames.count();
            t += (value*value*value - value)/12;
            double avgRang = 0;
            for (int i=0;i<sames.size();i++) {
                rang++;
                avgRang += rang;
            }
            avgRang /= sames.size();
            for (int i=0;i<sames.size();i++)
                results[sames[i]] = avgRang;
        }
        else {
            rang++;
            results[sames[0]] = rang;
        }
    }

    return results;
}

// 4    1  4    7    3 9 8 7       5
// 3.5  1  3.5  6.5  2 9 8 6.5     5

void SpearmanAnalizer::printResults(QTableWidget * table) {
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

void SpearmanAnalizer::fillInputData() {
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
