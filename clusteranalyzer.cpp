#include "clusteranalyzer.h"
#include "statisticanalyzedialog.h"
#include <QSpinBox>
#include "math.h"

ClusterAnalyzer::ClusterAnalyzer()
{
}

void ClusterAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    for (int i=0;i<ClustersNumber;i++) {
        QList<double> row;
        for (int j=0; j<matrix[0].size();j++) {
            row.append(matrix[i][j]);
        }
        if (checkHasSameCluster(row)) {
            continue;
        }
        clusters.append(row);
    }

    fillClusters();
    for (int i=0;true;i++) {
        clusters = findNewClusters();
        fillClusters();
        if (checkClustersCounts())
            break;
    }

}

bool ClusterAnalyzer::checkHasSameCluster(QList<double> cluster) {
    bool result = false;
    for (int i=0;i<clusters.size();i++) {
        bool check = true;
        for (int j=0;j<cluster.size();j++)
            if (clusters[i][j] != cluster[j]) {
                check = false;
                break;
            }
        if (check) {
            result = true;
            break;
        }
    }
    return result;
}

void ClusterAnalyzer::fillClusters() {
    prevClustersFill = clustersFill;
    clustersFill.clear();

    for (int i=0;i<clusters.size();i++) {
        QList<int> l;
        clustersFill.append(l);
    }
    for (int row=0;row<matrix.size();row++) {
        double minLen = getDistance(clusters[0],matrix[row]);
        int nearest = 0;
        for (int clusterIdx=1;clusterIdx<clusters.size();clusterIdx++) {
            double dist = getDistance(clusters[clusterIdx], matrix[row]);
            if (dist < minLen) {
                minLen = dist;
                nearest = clusterIdx;
            }
        }
        clustersFill[nearest].append(row);
    }
}

bool ClusterAnalyzer::checkClustersCounts() {
    bool result = true;
    for (int i=0;i<clustersFill.size();i++) {
        for (int j=0;j<clustersFill[i].size();j++) {
            if (clustersFill[i][j] != prevClustersFill[i][j]) {
                result = false;
                break;
            }
        }
        if (!result)
            break;
    }
    return result;
}

QList<QList<double> > ClusterAnalyzer::findNewClusters() {
    QList<QList<double> > newClusters;
    for (int i=0;i<clusters.count();i++) {
        QList<double> newCoords;
        for (int j=0;j<clusters[i].size();j++) {
            double temp = 0.0;
            for (int pointIdx=0;pointIdx<clustersFill[i].size();pointIdx++) {
                temp += matrix[clustersFill[i][pointIdx]][j];
            }
            temp /= clustersFill[i].size();
            newCoords.append(temp);
        }
        newClusters.append(newCoords);
    }
    return newClusters;
}

double ClusterAnalyzer::getDistance(QList<double> from, QList<double> to) {
    double result = 0.0;
    for (int i=0;i<from.size();i++)
        result += (from[i]-to[i])*(from[i]-to[i]);
    return sqrt(result);
}

void ClusterAnalyzer::printResults(QTableWidget * table) {
    table->setColumnCount(2);
    table->setRowCount(clustersFill.size());

    for (int i=0;i<clustersFill.size();i++) {
        QString l = "";
        for (int j=0;j<clustersFill[i].size();j++) {
            if (j!=0)
                l += ", ";
            l += QString::number(clustersFill[i][j]+1);
        }
        table->setItem(i,0,new QTableWidgetItem(QString::number(i+1) + tr(" кластер")));
        table->setItem(i,1,new QTableWidgetItem(l));
    }

}

bool ClusterAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    dialog->SetDialogName(name);
    dialog->addAdditionalParam(new QSpinBox(), "Количество кластеров");

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
    ClustersNumber = dialog->GetIntParam("Количество кластеров");
    return true;
}

void ClusterAnalyzer::fillInputData() {
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

    matrix = tempMatrix;

    /*for(int col=0;col<tempMatrix.at(0).count();col++) {
        QList<double> colList;
        for (int row=0;row<tempMatrix.count();row++) {
            colList.append(tempMatrix.at(row).at(col));
        }
        matrix.append(colList);
    }*/
}

QHash<QString,QString> ClusterAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    for (int i=0;i<columnsCount;i++) {
        QString headerItemText = headerList.at(i);
        result.insert(QString::number(i), headerItemText);
    }
    return result;
}
