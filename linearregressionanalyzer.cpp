#include "linearregressionanalyzer.h"
#include "math.h"

LinearRegressionAnalyzer::LinearRegressionAnalyzer()
{
}

void LinearRegressionAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    QList<double> yList = matrix[0];
    QList<QList<double> > xMatrix;
    for (int i=1;i<matrix.size();i++) {
        xMatrix.append(matrix[i]);
    }

    QList<QList<double> > xNMatrix;
    for (int i=0;i<xMatrix.size();i++) {
        QList<double> xNiMatrix;
        double avg = average(xMatrix[i]);
        QList<double> razn;
        for (int j=0;j<xMatrix[i].size();j++) {
            razn.append(fabs(xMatrix[i][j] - avg));
        }
        double R = maxElement(razn);
        for (int j=0;j<xMatrix[i].size();j++)
            xNiMatrix.append((xMatrix[i][j] - avg)/R);
        xNMatrix.append(xNiMatrix);
    }

    QList<double> bMatrix;
    double temp = 0.0;
    for (int i=0;i<yList.size();i++)
        temp += yList[i];
    bMatrix.append(temp);
    for (int i=0;i<xNMatrix.size();i++) {
        temp = 0.0;
        for (int j=0;j<xNMatrix[i].size();j++) {
            temp += yList[i] * xNMatrix[i][j];
        }
        bMatrix.append(temp);
    }

    QList<QList<double> > aMatrix;
    QList<double> firstRow;
    firstRow.append(yList.size());
    for (int i=0;i<xNMatrix.size();i++)
        firstRow.append(sumElements(xNMatrix[i]));
    aMatrix.append(firstRow);
    for (int row=0;row<xNMatrix.size();row++) {
        QList<double> tempRow;
        for (int col=0;col<bMatrix.size();col++) {
            if (col<=row) {
                tempRow.append(0.0);
            }
            else {
                if (col>0) {
                    tempRow.append(sumListsElements(xNMatrix[col-1], xNMatrix[row]));
                }
            }
        }
        aMatrix.append(tempRow);
    }

    for (int row = 0;row<aMatrix.size();row++) {
        for (int col = 0; col < aMatrix[row].size();col++) {
            if (row <= col)
                continue;
            aMatrix[row][col] = aMatrix[col][row];
        }
    }

    results = Gauss(aMatrix, bMatrix);


    QList<double> y2List;
    for (int i=0;i<xNMatrix[0].size();i++) {
        temp = 0.0;
        for (int j= 0;j<results.size();j++) {
            if (j<results.size()-1)
                temp += results[j] * xNMatrix[j][i];
            else
                temp += results[j];
        }
        y2List.append(temp);
    }

    double y2Avg = average(y2List);
    double yAvg = average(yList);

    double RUp = 0.0, RDown = 0.0;
    for (int i=0;i<yList.size();i++) {
        RUp += (y2List[i]-y2Avg)*(y2List[i]-y2Avg);
        RDown += (yList[i] - yAvg)*(yList[i] - yAvg);
    }

    R2 = RUp/RDown;
}

QList<double> LinearRegressionAnalyzer::Gauss(QList<QList<double> > a, QList<double> y) {
    int n = y.size();
    QList<double> x = createNullList(n);

    QList<QList<double> > c = createNullMatrix(n);
    QList<double> x1 = createNullList(n);
    QList<double> d = createNullList(n);

    double m,l,e=0.01;
    int f,i,j;

    for(i=0;i<n;i++)
    {
        d[i]=y[i]/a[i][i];
        for(j=0;j<n;j++)
        {
            c[i][j]=-a[i][j]/a[i][i];
        }
    }
    f=1;
    while(f==1)
    {

        for(i=0;i<n;i++)
        {
            m=0;l=0;
            for(j=0;j<=i-1;j++)
            {
                m=m+c[i][j]*x1[j];
            }
            for(j=i+1;j<n;j++)
            {
                l=l+c[i][j]*x[j];
            }
            x1[i]=m+l+d[i];
        }
        f=0;
        for(i=0;i<n;i++)
        {
            if( fabs(x[i]-x1[i])>e)
                f=1;
            x[i]=x1[i];
        }
    }
    return x;
}

void LinearRegressionAnalyzer::printResults(QTableWidget * table) {
    table->setColumnCount(1);
    table->setRowCount(2);

    QString l1 = "y''=";
    for (int i=0;i<results.size();i++) {
        if (i!= 0)
            l1 += " + ";
        l1 += QString::number(results[i]);
        if (i<results.size()-1)
            l1 += "X" + QString::number(i+1);
    }
    QString l2 = tr("Качество оценивания: ") + QString::number(R2);
    table->setItem(0,0,new QTableWidgetItem(l1));
    table->setItem(1,0,new QTableWidgetItem(l2));
}

void LinearRegressionAnalyzer::fillInputData() {
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
