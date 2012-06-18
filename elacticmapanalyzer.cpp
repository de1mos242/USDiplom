#include "elacticmapanalyzer.h"
#include "statisticanalyzedialog.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "componentanalyzer.h"
#include "vector.h"
#include "graphic3d.h"

ElacticMapAnalyzer::ElacticMapAnalyzer()
{
}

void ElacticMapAnalyzer::fillInputData() {
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

QHash<QString,QString> ElacticMapAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    for (int i=0;i<columnsCount;i++) {
        QString headerItemText = headerList.at(i);
        result.insert(QString::number(i), headerItemText);
    }
    return result;
}

bool ElacticMapAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    dialog->SetDialogName(name);
    dialog->addAdditionalParam(new QSpinBox(), tr("Количество итераций"));
    dialog->addAdditionalParam(new QSpinBox(), tr("P"));
    dialog->addAdditionalParam(new QSpinBox(), tr("Q"));
    QDoubleSpinBox* muSP = new QDoubleSpinBox();
    muSP->setMaximum(10000);
    dialog->addAdditionalParam(muSP, tr("Mu"));
    QDoubleSpinBox* lambdaSP = new QDoubleSpinBox();
    lambdaSP->setMaximum(10000);
    dialog->addAdditionalParam(lambdaSP, tr("Lambda"));

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
    p = dialog->GetIntParam("P");
    q = dialog->GetIntParam("Q");
    pq = p*q;
    iterations = dialog->GetIntParam(tr("Количество итераций"));
    lambda = dialog->GetDoubleParam("Lambda");
    mu = dialog->GetDoubleParam("Mu");
    return true;
}

void ElacticMapAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    ComponentAnalyzer* component = new ComponentAnalyzer();
    component->table = this->table;
    component->parametersList = parametersList;
    component->fillInputTableData();
    component->DoAnalyze();
    TMatrix = component->TMatrix;

    fillInputPoints();
    initTaxons();
    taxonsCount = getTaxonsCount();
    for (int i=0;i<iterations;i++) {
        fillAMatrix();
        fillBMatrix();
        findNewTaxons();
        if (compareTaxonsCount())
            break;
    }

}

void ElacticMapAnalyzer::printResults(QTableWidget * table) {
    table->setObjectName(" Таксоны");
    table->setEditTriggers(QTableWidget::NoEditTriggers);

    table->setColumnCount(AMatrix[0].size());
    table->setRowCount(AMatrix.size());

    for (int i=0;i<table->columnCount();i++)
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(QString::number(i)));
    for (int i=0;i<AMatrix.size();i++)
        for (int j=0;j<AMatrix[i].size();j++)
            table->setItem(i,j,new QTableWidgetItem(QString::number(AMatrix[i][j])));
    int startRow = table->rowCount();
    table->setRowCount(table->rowCount()+prevTaxonsCountHistory[0].size());
    for(int i=0;i<prevTaxonsCountHistory[0].size();i++) {
        table->setItem(startRow+i,0,new QTableWidgetItem(QString::number(prevTaxonsCountHistory[0][i])));
    }

    startRow = table->rowCount();
    table->setRowCount(table->rowCount()+taxons.size());
    for(int i=0;i<taxons.size();i++) {
        table->setItem(startRow+i,0,new QTableWidgetItem(QString::number(taxons[i].x)));
        table->setItem(startRow+i,1,new QTableWidgetItem(QString::number(taxons[i].y)));
        table->setItem(startRow+i,2,new QTableWidgetItem(QString::number(taxons[i].z)));
    }

    startRow = table->rowCount();
    table->setRowCount(table->rowCount()+BMatrixOld.size());
    for(int i=0;i<BMatrixOld.size();i++) {
        table->setItem(startRow+i,0,new QTableWidgetItem(QString::number(BMatrixOld[i].x)));
        table->setItem(startRow+i,1,new QTableWidgetItem(QString::number(BMatrixOld[i].y)));
        table->setItem(startRow+i,2,new QTableWidgetItem(QString::number(BMatrixOld[i].z)));
    }

    Point3D coords;
    coords.x = 5.0f;
    coords.y = 5.0f;
    coords.z = 5.0f;
    QList<QString> coordLables;
    coordLables.append("PC1");
    coordLables.append("PC2");
    coordLables.append("PC3");

    QWidget * tab1 = AdditionalWidgets.at(0);
    tab1->setObjectName("График");
    Graphic3d * g1 = new Graphic3d(tab1);
    //g1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    coordLables.append(headerList.at(parametersList->at(0).toInt()));
    coordLables.append(headerList.at(parametersList->at(1).toInt()));
    coordLables.append(headerList.at(parametersList->at(2).toInt()));
    g1->SetCoordsData(coords, coordLables);
    g1->SetPoints(getTmatrix());
    g1->SetNet(getNet());
    tab1->setLayout(new QGridLayout());
    tab1->layout()->addWidget(g1);
}

QList<QList<Point3D> > ElacticMapAnalyzer::getNet() {
    QList<QList<Point3D> > result;
    for (int i=0;i<p;i++) {
        QList<Point3D> row;
        for (int j=0;j<q;j++) {
            row.append(taxons[i*q+j]);
        }
        result.append(row);
    }
    return result;
}

QList<Point3DEx> ElacticMapAnalyzer::getTmatrix() {
    QList<Point3DEx> result;
    for (int i=0;i<TMatrix.count();i++) {
        Point3DEx p;
        p.x = TMatrix[i][0];
        p.y = TMatrix[i][1];
        p.z = TMatrix[i][2];
        p.label = QString::number(i+1);
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}

void ElacticMapAnalyzer::fillInputPoints() {
    for (int row=0;row<TMatrix.count();row++) {
        Point3D p;
        p.x = TMatrix[row][0];
        p.y = TMatrix[row][1];
        p.z = TMatrix[row][2];
        inputPoints.append(p);
    }
}

void ElacticMapAnalyzer::fillAMatrix() {
    AMatrix.clear();
    for (int s=0;s<pq;s++) {
        QList<double> row;
        for (int t=0;t<pq;t++) {
            int i, j, k, l;
            i = (((s+1)-1)/q) + 1;
            j = (s+1) - (((s+1)-1)/q)*q;
            k = (((t+1)-1)/q) + 1;
            l = (t+1) - (((t+1)-1)/q)*q;

            double res = 0.0;
            if (i==k && j==l)
                res = findAElement(1,k,l);
            else if (i==k-1 && j==l)
                res = findAElement(2,k,l);
            else if (i==k+1 && j==l)
                res = findAElement(3,k,l);
            else if (i==k-2 && j==l)
                res = findAElement(4,k,l);
            else if (i==k+2 && j==l)
                res = findAElement(5,k,l);
            else if (i==k && j==l-2)
                res = findAElement(6,k,l);
            else if (i==k && j==l-1)
                res = findAElement(7,k,l);
            else if (i==k && j==l+1)
                res = findAElement(8,k,l);
            else if (i==k && j==l+2)
                res = findAElement(9,k,l);
            row.append(res);
        }
        AMatrix.append(row);
    }
}

double ElacticMapAnalyzer::findAElement(int type, int k, int l) {
    double a1, a2, a3, a4, a5, a6, a7;
    switch(type) {
    case 1:
        a1 = taxonsCount[(k-1)*q+(l-1)]/(double)TMatrix.size();
        a2 = (lambda/pq)*(4 - kron(k,1) - kron(k,p) - kron(l,1) - kron(l,q));
        a3 = (1-kron(k,2))*(1-kron(k,1))+(1-kron(k,p-1))*(1-kron(k,p))-2*(1-kron(k,p))*(1-kron(k,1));
        a4 = (1-kron(l,2))*(1-kron(l,1))+(1-kron(l,q-1))*(1-kron(l,q))-2*(1-kron(l,q))*(1-kron(l,1));
        a5 = (mu/pq);
        a6 = a5 * (a3 + a4);
        a7 = a1 + a2 + a6;
        return a7;
        //return taxonsCount[(k-1)*q+(l-1)]/(double)TMatrix.size() +
          //      (lambda/pq)*(4 - kron(k,1) - kron(k,p) - kron(l,1) - kron(l,q)) +
            //    (mu/pq)*((1-kron(k,2))*(1-kron(k,1))+(1-kron(k,p-1))*(1-kron(k,p))-2*(1-kron(k,p))*(1-kron(k,l)) +
              //           (1-kron(l,2))*(1-kron(l,1))+(1-kron(l,q-1))*(1-kron(l,q))-2*(1-kron(l,q))*(1-kron(l,1)));
    case 2:
        return (lambda/pq)*(kron(l,1)-1) + (2*mu/pq)*(kron(l,2)-1)*(1-kron(l,1));
    case 3:
        return (lambda/pq)*(kron(l,q)-1) + (2*mu/pq)*(kron(l,q-1)-1)*(1-kron(l,q));
    case 4:
        return (mu/pq)*(1-kron(l,2))*(1-kron(l,1));
    case 5:
        return (mu/pq)*(1-kron(l,q-1))*(1-kron(l,q));
    case 6:
        return (mu/pq)*(1-kron(k,2))*(1-kron(k,1));
    case 7:
        return (lambda/pq)*(kron(k,1)-1) + (2*mu/pq)*(kron(k,2)-1)*(1-kron(k,1));
    case 8:
        return (lambda/pq)*(kron(k,p)-1) + (2*mu/pq)*(kron(k,p-1)-1)*(1-kron(k,p));
    case 9:
        return (mu/pq)*(1-kron(k,p-1))*(1-kron(k,p));
    }
    return 0.0;
}

int ElacticMapAnalyzer::kron(int a, int b) {
    return (a==b)?1:0;
}

void ElacticMapAnalyzer::initTaxons() {
    for (int pidx=0;pidx<p;pidx++)
        for(int qidx=0;qidx<q;qidx++) {
            Point3D px;
            px.x = pidx/10.0;
            px.y = qidx/10.0;
            px.z = pidx/10.0;
            px.x -= p/20.0;
            px.y -= q/20.0;
            px.z -= p/20.0;
            taxons.append(px);
        }
}

QList<int> ElacticMapAnalyzer::getTaxonsCount() {
    QList<int> result;

    for (int i=0;i<taxons.size();i++) {
        result.append(findTaxonPoints(i).size());
    }
    return result;
}

QList<Point3D> ElacticMapAnalyzer::findTaxonPoints(int idx) {
    QList<Point3D> result;

    QHash<int, int> pointInTaxon;
    for (int i=0;i<taxonsCount.size();i++)
        pointInTaxon.insert(i, 0);
    for (int i=0;i<inputPoints.size();i++) {
        int nearest = 0;
        double minLen = 1005000.0;
        for (int j=1;j<taxons.size();j++) {
            Vector v = Vector::Create(inputPoints[i], taxons[j]);
            double len = v.length();
            if (len < minLen) {
                nearest = j;
                minLen = len;
            }
        }
        if (nearest == idx)
            result.append(inputPoints[i]);
    }
    return result;
}

void ElacticMapAnalyzer::fillBMatrix() {
    BMatrix.clear();
    for (int s=0;s<pq;s++) {
        int i = (((s+1)-1)/q) + 1;
        int j = (s+1) - (((s+1)-1)/q)*q;
        QList<Point3D> pointsInTaxon = findTaxonPoints((i-1)*q+(j-1));
        int pointsCount = pointsInTaxon.size();
        Point3D p;
        for (int d=0;d<pointsCount;d++) {
            p.x += pointsInTaxon[d].x;
            p.y += pointsInTaxon[d].y;
            p.z += pointsInTaxon[d].z;
        }
        /*if (pointsCount > 0) {
            p.x /= pointsCount;
            p.y /= pointsCount;
            p.z /= pointsCount;
        }*/
        BMatrix.append(p);
    }
}

bool ElacticMapAnalyzer::compareTaxonsCount() {
    bool res = true;
    foreach(QList<int> oldCounter, prevTaxonsCountHistory) {
        res = true;
        for (int i=0;i<taxonsCount.size();i++)
            if (taxonsCount[i] != oldCounter[i]) {
                res = false;
                break;
            }
        if (res)
            break;
    }
    return res;
}

void ElacticMapAnalyzer::findNewTaxons() {
    prevTaxonsCountHistory.append(taxonsCount);
    taxonsCount.clear();
    taxons.clear();

    QList<QList<double> > triangle = findTriangleAMatrix();

    for (int i=0;i<triangle.size();i++) {
        Point3D p;
        taxons.append(p);
    }

    for (int row=triangle.count()-1;row>=0;row--) {
        Point3D p;
        p.x = BMatrix[row].x;
        p.y = BMatrix[row].y;
        p.z = BMatrix[row].z;
        for (int col=row+1;col<triangle.size();col++) {
            p.x -= triangle[row][col] * taxons[col].x;
            p.y -= triangle[row][col] * taxons[col].y;
            p.z -= triangle[row][col] * taxons[col].z;
        }
        p.x /= triangle[row][row];
        p.y /= triangle[row][row];
        p.z /= triangle[row][row];

        taxons[row] = p;
    }

    taxonsCount = getTaxonsCount();
}

QList<QList<double> > ElacticMapAnalyzer::findTriangleAMatrix() {
    QList<QList<double> > result = AMatrix;
    BMatrixOld = BMatrix;

    for (int col = 0;col<result[0].size()-1;col++) {
        for (int row = col+1;row < result.size();row++) {
            double temp = result[row][col]/result[col][col];
            for (int incol=0;incol<result.size();incol++) {
                result[row][incol]-=temp*result[col][incol];
            }
            BMatrix[row].x -= temp * BMatrix[col].x;
            BMatrix[row].y -= temp * BMatrix[col].y;
            BMatrix[row].z -= temp * BMatrix[col].z;

        }
    }

    return result;
}
