#include "statisticanalyzer.h"
#include "baseanalyzer.h"
#include <QMessageBox>
#include <QDialog>
#include <qmath.h>

StatisticAnalyzer::StatisticAnalyzer():BaseAnalyzer()
{
}
void StatisticAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();


    results = new QHash<int,QHash<QString, double>* >();
    for (int i=0;i<columnsCount;i++)
        calcColumn(i);
}

void StatisticAnalyzer::calcColumn(int columnIdx) {
    QHash<QString,double> *column = new QHash<QString,double>();
    for (int i=0;i<parametersList->count();i++)
    {
        QString parameter = parametersList->at(i);
        double result;
        if (parameter == "Average")
            result = calcAverage(columnIdx);
        else if (parameter == "Variance")
            result = calcVariance(columnIdx);
        else if (parameter == "Max")
            result = calcMax(columnIdx);
        else if (parameter == "Min")
            result = calcMin(columnIdx);
        else if (parameter == "StandardDeviation")
            result = calcStandardDeviation(columnIdx);
        else if (parameter == "Median")
            result = calcMedian(columnIdx);
        else if (parameter == "Quartile25")
            result = calcQuartile25(columnIdx);
        else if (parameter == "Quartile75")
            result = calcQuartile75(columnIdx);
        else if (parameter == "Mode")
            result = calcMode(columnIdx);
        else if (parameter == "Asymmetry")
            result = calcAsymmetry(columnIdx);
        else if (parameter == "Excess")
            result = calcExcess(columnIdx);

        column->insert(parameter, result);
    }
    results->insert(columnIdx, column);
}

double StatisticAnalyzer::calcAverage(int idx) {
    QList<double> * column = data->value(idx);
    double sum = 0;
    for (int i=0;i<column->count();i++)
        sum+=column->at(i);
    return (sum/column->count());
}

double StatisticAnalyzer::calcMin(int idx) {
    QList<double> * column = data->value(idx);
    double min = column->at(0);
    for (int i=1;i<column->count();i++)
        if (column->at(i) < min)
            min = column->at(i);
    return (min);
}

double StatisticAnalyzer::calcMax(int idx) {
    QList<double> * column = data->value(idx);
    double max = column->at(0);
    for (int i=1;i<column->count();i++)
        if (column->at(i) > max)
            max = column->at(i);
    return (max);
}

double StatisticAnalyzer::calcAsymmetry(int idx) {
    QList<double> * column = data->value(idx);
    double average = calcAverage(idx);
    double standartDeriviation = calcStandardDeviation(idx);
    double sum = 0;
    for (int i=0;i<column->count();i++) {
        double temp =(column->at(i) - average);
        sum += temp*temp*temp;
    }
    double m = sum/column->count();
    return m/(standartDeriviation*standartDeriviation*standartDeriviation);
}

double StatisticAnalyzer::calcExcess(int idx) {
    QList<double> * column = data->value(idx);
    double average = calcAverage(idx);
    double standartDeriviation = calcStandardDeviation(idx);
    double sum = 0;
    for (int i=0;i<column->count();i++) {
        double temp =(column->at(i) - average);
        sum += temp*temp*temp*temp;
    }
    double m = sum/column->count();
    return (m/(standartDeriviation*standartDeriviation*standartDeriviation*standartDeriviation))-3;
}

double StatisticAnalyzer::calcMedian(int idx) {
    QList<double> * column = data->value(idx);
    QList<double> * newArray = sortArray(column);

    int n = (newArray->count()+1)/2;
    if (newArray->count()%2==0) {
        return (newArray->at(n-1)+newArray->at(n))/2.;
    }
    else {
        return (newArray->at(n-1));
    }
}

double StatisticAnalyzer::calcMode(int idx) {
    QList<double> * newArray = sortArray(data->value(idx));
    int currentCount = 0, maxCount = 0;
    double currentVal = newArray->at(0),maxVal = newArray->at(0);
    for (int i=1;i<newArray->count();i++) {
        double val = newArray->at(i);
        if (val != currentVal) {
            if (currentCount > maxCount) {
                maxCount = currentCount;
                maxVal = currentVal;
            }
            currentCount = 0;
        }
        else {
            currentCount++;
        }
        currentVal = val;
    }
    return maxVal;
}

double StatisticAnalyzer::calcQuartile25(int idx) {
    QList<double> * column = data->value(idx);
    QList<double> * newArray = sortArray(column);
    int median = (newArray->count()+1)/2;
    if (newArray->count()%2!=0)
        median--;
    int n = (median+1)/2;
    if (median%2==0) {
        return (newArray->at(n-1)+newArray->at(n))/2.;
    }
    else {
        return (newArray->at(n-1));
    }
}

// 1 2 3 4 5 6 7 (8)
double StatisticAnalyzer::calcQuartile75(int idx) {
    QList<double> * column = data->value(idx);
    QList<double> * newArray = sortArray(column);
    int median = (newArray->count()+1)/2;
    if (newArray->count()%2!=0)
        median--;
    int n = (median+1)/2;
    if (median%2==0) {
        return (newArray->at(n-1+median)+newArray->at(n+median))/2.;
    }
    else {
        return (newArray->at(n+median));
    }
}

double StatisticAnalyzer::calcStandardDeviation(int idx) {
    double variance = calcVariance(idx);
    return sqrt(variance);
}

double StatisticAnalyzer::calcVariance(int idx) {
    QList<double> * column = data->value(idx);
    double average = calcAverage(idx);
    double sum = 0;
    for (int i=0;i<column->count();i++) {
        double temp =(column->at(i) - average);
        sum += temp*temp;
    }
    return sum/column->count();
}

QList<double>* StatisticAnalyzer::sortArray(QList<double> *source) {
    QList<double> * newArray = new QList<double>();
    for (int i=0;i<source->count();i++)
        newArray->append(source->at(i));

    bool dirty = true;
    while(dirty) {
        dirty=false;
        for (int i=0;i<newArray->count()-1;i++) {
            if (newArray->at(i) > newArray->at(i+1)) {
                newArray->swap(i,i+1);
                dirty = true;
            }
        }
    }
    return newArray;
}

void StatisticAnalyzer::printResults(QTableWidget *table) {
    QHash<QString, QString> localNames = getAllParams();

    table->setColumnCount(columnsCount);
    table->setRowCount(parametersList->count());
    for (int i=0;i<headerList.count();i++)
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(i)));
    for (int i=0;i<parametersList->count();i++)
        table->setVerticalHeaderItem(i, new QTableWidgetItem(localNames[parametersList->at(i)]));
    for (int col=0;col<columnsCount;col++) {
        for (int row = 0;row<parametersList->count();row++) {
            QString text = QString::number(results->value(col)->value(parametersList->at(row)));
            table->setItem(row, col, new QTableWidgetItem(text));
        }
    }
}

QHash<QString, QString> StatisticAnalyzer::getAllParams() {
    QHash<QString,QString> params; // = new QHash<QString,QString>();
    params.insert("Average","Среднее по данному параметру");
    params.insert("Variance","Дисперсия");
    params.insert("Max","Максимум");
    params.insert("Min","Минимум");
    params.insert("StandardDeviation","Стандартное отклонение");
    params.insert("Median","Медиана");
    params.insert("Quartile25","Квартиль 25%");
    params.insert("Quartile75","Квартиль 75%");
    params.insert("Mode","Мода");
    params.insert("Asymmetry","Ассиметрия");
    params.insert("Excess","Эксцесс");
    return params;
}
