#include "pairregressionanalyzer.h"
#include "pairregressionanalyzergraphic.h"

PairRegressionAnalyzer::PairRegressionAnalyzer()
{
}

void PairRegressionAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    QList<double> xList = matrix[1];
    QList<double> yList = matrix[0];
    double xAvg = average(xList);
    double yAvg = average(yList);

    double bUp = 0.0;
    double bDown = 0.0;
    for (int i=0;i<xList.size();i++) {
        bUp += (xList[i]-xAvg)*(yList[i]-yAvg);
        bDown += (xList[i]-xAvg)*(xList[i]-xAvg);
    }
    double b = bUp/bDown;
    double a = yAvg - b * xAvg;
    this->A = a;
    this->B = b;

    QList<double> y2List;
    for (int i=0;i<xList.size();i++) {
        y2List.append(b*xList[i] + a);
    }

    results.append(xList);
    results.append(yList);
    results.append(y2List);

    double y2Avg = average(y2List);

    double RUp = 0.0;
    double RDown = 0.0;
    for (int i=0;i<y2List.size();i++) {
        RUp += (y2List[i] - y2Avg)*(y2List[i] - y2Avg);
        RDown += (yList[i] - yAvg)*(yList[i] - yAvg);
    }
    R = RUp/RDown;
}

void PairRegressionAnalyzer::printResults(QTableWidget * table) {
    table->setColumnCount(results.count());
    table->setRowCount(results[0].count());

    table->setHorizontalHeaderItem(0,new QTableWidgetItem("x"));
    table->setHorizontalHeaderItem(1,new QTableWidgetItem("y"));
    table->setHorizontalHeaderItem(2,new QTableWidgetItem("y''"));

    for(int col=0;col<results.count();col++)
        for(int row=0;row<results[col].count();row++)
            table->setItem(row,col,new QTableWidgetItem(QString::number(results[col][row])));

    Point3D coords;
    coords.x = 5.0f;
    coords.y = 5.0f;
    coords.z = 5.0f;
    Point3D coordsScale;
    coordsScale.x = 5.0/maxElement(results[0]);
    coordsScale.y = 5.0/maxElement(results[1]);
    coordsScale.z = 1;
    QList<QString> coordLables;

    QWidget * tab1 = AdditionalWidgets.at(0);
    tab1->setObjectName("График");
    PairRegressionAnalyzerGraphic * g1 = new PairRegressionAnalyzerGraphic(tab1);
    coordLables.append("");
    coordLables.append("");
    g1->SetCoordsData(coords, coordLables,Figure3D::black, Figure3D::darkGreen, &coordsScale);
    g1->SetPoints(getYPoints());
    g1->SetLinePoints(getY2Points());
    g1->SetErrorText("Качество оценивания: " + QString::number(R));
    g1->SetEquation("y'' = " + QString::number(A) + " + " + QString::number(B) + "x");
    tab1->setLayout(new QGridLayout());
    tab1->layout()->addWidget(g1);
}

void PairRegressionAnalyzer::fillInputData() {
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

QList<Point3DEx> PairRegressionAnalyzer::getYPoints() {
    double xScale = 5.0/maxElement(results[0]);
    double yScale = 5.0/maxElement(results[1]);
    QList<Point3DEx> result;
    for (int i=0;i<results[0].size(); i++) {
        Point3DEx p;
        p.x = results[0][i]*xScale;
        p.y = results[1][i]*yScale;
        p.label = QString::number(i+1);
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}

QList<Point3D> PairRegressionAnalyzer::getY2Points() {
    double xScale = 5.0/maxElement(results[0]);
    double yScale = 5.0/maxElement(results[1]);
    QList<Point3D> result;
    for (int i=0;i<results[0].size();i++) {
        Point3D p;
        p.x = results[0][i]*xScale;
        p.y = results[2][i]*yScale;
        result.append(p);
    }
    return result;
}
