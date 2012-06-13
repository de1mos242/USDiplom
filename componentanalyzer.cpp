#include "componentanalyzer.h"
#include "QTableWidgetItem"
#include <QTableWidget>
#include <QHeaderView>
#include <QMatrix>
#include <qmath.h>
#include <QGridLayout>
#include "glwidget.h"
#include "graphnode.h"
#include "vector.h"

ComponentAnalyzer::ComponentAnalyzer()
{
    e1 = 0.5;
    e2 = 0.4;
    ShowGraph = false;
}

void ComponentAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();

    for (int i=0;i<matrix.count();i++) {
        QList<double> tRow;
        for(int j=0;j<matrix[i].count();j++)
            tRow.append(0);
        TMatrix.append(tRow);
    }

    for (int i=0;i<parametersList->count();i++) {
        QList<double> pRow;
        for(int j=0;j<parametersList->count();j++)
            pRow.append(0);
        PMatrix.append(pRow);
    }

    QList<QList<double> > Tknew;
    QList<QList<double> > Pkm;
    QList<QList<double> > Pkt;

    QList<double> Tk;
    for (int i=0;i<matrix.count();i++)
        Tk.append(matrix[i][0]);

    QList<QList<double> > workMatrix = copyMatrix(matrix);

    QList<QList<double> > Ttk;
    Ttk.append(Tk);
    QList<QList<double> > Tkm = transponate(Ttk);

    for (int componentIndex = 0;componentIndex<matrix[0].count();componentIndex++) {

        bool exit = true;
        while (exit) {
            Ttk = transponate(Tkm);

            Pkt = multiMatrix(Ttk, workMatrix);

            QList<QList<double> > temp = multiMatrix(Ttk,Tkm);
            for (int i=0;i<workMatrix[0].count();i++)
                Pkt[0][i] = Pkt[0][i]/temp[0][0];
            Pkm = transponate(Pkt);

            double norm = sqrt(multiMatrix(Pkt,Pkm)[0][0]);
            for (int i=0;i<Pkm.count();i++) {
                Pkt[0][i] = Pkt[0][i]/norm;
                Pkm[i][0] = Pkm[i][0]/norm;
            }

            Tknew = multiMatrix(workMatrix, Pkm);

            temp = multiMatrix(Pkt,Pkm);
            for (int i=0;i<Tknew.count();i++)
                Tknew[i][0] = Tknew[i][0]/temp[0][0];

            double checkE = 0;
            for (int i=0;i<Tknew.count();i++)
                checkE += (Tknew[i][0]-Tkm[i][0])*(Tknew[i][0]-Tkm[i][0]);
            checkE = sqrt(checkE);

            if (checkE < e1) {
                exit = false;
            }
            else {
                for (int i=0;i<Tkm.count();i++)
                    Tkm[i][0] = Tknew[i][0];
            }
        }

        QList<QList<double> > minus = multiMatrix(Tkm,Pkt);
        for (int i=0;i<workMatrix.count();i++)
            for (int j=0;j<workMatrix[i].count();j++)
                workMatrix[i][j] = workMatrix[i][j] - minus[i][j];

        for (int i=0;i<TMatrix.count();i++)
            TMatrix[i][componentIndex] = Tknew[i][0];
        for (int i=0;i<PMatrix.count();i++)
            PMatrix[i][componentIndex] = Pkm[i][0];
    }
}

void ComponentAnalyzer::printResults(QTableWidget * table) {
    int parametersCount = parametersList->count();
    table->setObjectName(tr("T Матрица"));
    table->setEditTriggers(QTableWidget::NoEditTriggers);

    table->setColumnCount(parametersCount);
    table->setRowCount(TMatrix.count());

    for (int i=0;i<parametersCount;i++) {
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<TMatrix.count();i++) {
        table->setVerticalHeaderItem(i, new QTableWidgetItem(this->table->verticalHeaderItem(i)->text()));
        for(int j=0;j<parametersCount;j++)
            table->setItem(i,j,new QTableWidgetItem(QString::number(TMatrix[i][j])));
    }


    QWidget * tab2 = AdditionalWidgets.at(0);
    tab2->setObjectName(tr("График"));
    GLWidget *gl = new GLWidget(tab2);
    gl->setCoords(prepareGraphicData());
    QList<QString> labels;
    for (int i = 0; i<parametersCount; i++)
        labels.append(headerList.at(parametersList->at(i).toInt()));
    gl->setCoordsLabels(labels);
    if (this->ShowGraph)
        gl->setGrahp(prepareGraphData());
    gl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}

void ComponentAnalyzer::fillInputData() {
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
            matrix.append(rowList);
        else
            skipRow = false;
    }
    getIMatrix();
    normalize();
}

QHash<QString,QString> ComponentAnalyzer::getAllParams() {
    QHash<QString,QString> result;
    for (int i=0;i<columnsCount;i++) {
        QString headerItemText = headerList.at(i);
        result.insert(QString::number(i), headerItemText);
    }
    return result;
}

void ComponentAnalyzer::normalize() {
    for (int col=0;col<iMatrix.size();col++) {
        double avg = average(iMatrix[col]);
        double deriviation = standardDeviation(iMatrix[col]);
        for (int row=0;row<iMatrix[col].size();row++) {
            double value = (iMatrix[col][row] - avg)/deriviation;
            value = roundValue(value);
            matrix[row][col] = value;
        }
    }
}

void ComponentAnalyzer::getIMatrix() {
    QList<QList<double> > temp;
    for (int col=0;col<matrix[0].size();col++) {
        QList<double> column;
        for (int row=0;row<matrix.size();row++) {
            column.append(matrix[row][col]);
        }
        temp.append(column);
    }
    iMatrix = temp;
}

void ComponentAnalyzer::getITmatrix() {
    QList<QList<double> > temp;
    for (int col=0;col<TMatrix[0].size();col++) {
        QList<double> column;
        for (int row=0;row<TMatrix.size();row++) {
            column.append(TMatrix[row][col]);
        }
        temp.append(column);
    }
    /*for (int col = temp.size();col <=3;col++) {
        QList<double> column;
        for (int row=0;row<TMatrix.size();row++)
            column.append(0.0f);
        temp.append(column);
    }*/
    iTMatrix = temp;
}

QList<QList<QPair<QString, double> > > ComponentAnalyzer::prepareGraphicData() {
    QList<QList<QPair<QString, double> > > temp;
    for (int col=0;col<TMatrix[0].size();col++) {
        QList<QPair<QString, double> > column;
        for (int row=0;row<TMatrix.size();row++) {
            QPair<QString,double> pair;
            pair.first = this->table->verticalHeaderItem(row)->text();
            pair.second = TMatrix[row][col];
            column.append(pair);
        }
        temp.append(column);
    }
    return temp;
}

double ComponentAnalyzer::getCloudCenterByCoord(double one, double two) {
    double from, to;
    if (one > two) {
        from = two;
        to = one;
    }
    else {
        from = one;
        to = two;
    }

    double delta = (to - from)/2;
    return from + delta;
}

GraphNode* ComponentAnalyzer::prepareGraphData() {
    getITmatrix();


    graphNodePoints.append(getPointInCloud(iTMatrix));

    GraphNode* temp = new GraphNode();
    QList<QList<double> > workSector = iTMatrix;

    for (int i=0;i<5;i++) {
        workSector = getMaxCloudFromSectors(getSectors(workSector));
        graphNodePoints.append(getPointInCloud(workSector));
    }

    GraphNode *pNode = temp;
    for(int i=0;i<graphNodePoints.size();i++) {
        Point3D point = graphNodePoints[i];
        pNode->coords.append(point.x);
        pNode->coords.append(point.y);
        pNode->coords.append(point.z);

        if (i < graphNodePoints.size()-1) {
            GraphNode *cNode = new GraphNode();
            pNode->nodes.append(cNode);
            pNode = cNode;
        }
    }

    return temp;
}

QList<QList<QList<double> > > ComponentAnalyzer::getSectors(QList<QList<double> > cloud) {
    QList<QList<QList<double> > > result;

    Point3D center = getCloudCenter(cloud);

    if (cloud.size() == 1) {
        QList<QList<double> > xL;
        QList<QList<double> > xM;
        for (int i = 0; i< cloud[0].size();i++) {
            QList<double> line;
            for (int j=0;j<cloud.size();j++)
                line.append(cloud[j][i]);
            if (cloud[0][i] <= center.x)
                xL.append(line);
            else
                xM.append(line);
        }
        result.append(xL);
        result.append(xM);
    }
    else if (cloud.size() == 2) {
        QList<QList<double> > xLyL;
        QList<QList<double> > xMyL;
        QList<QList<double> > xLyM;
        QList<QList<double> > xMyM;
        for (int i = 0; i< cloud[0].size();i++) {
            QList<double> line;
            for (int j=0;j<cloud.size();j++)
                line.append(cloud[j][i]);
            if (cloud[0][i] <= center.x) {
                if (cloud[1][i] <= center.y)
                    xLyL.append(line);
                else
                    xLyM.append(line);
            }
            else {
                if (cloud[1][i] <= center.y)
                    xMyL.append(line);
                else
                    xMyM.append(line);
            }
        }
        result.append(xLyL);
        result.append(xLyM);
        result.append(xMyL);
        result.append(xMyM);
    }
    else if (cloud.size() > 2) {
        QList<QList<double> > xLyLzL;
        QList<QList<double> > xMyLzL;
        QList<QList<double> > xLyMzL;
        QList<QList<double> > xMyMzL;
        QList<QList<double> > xLyLzM;
        QList<QList<double> > xMyLzM;
        QList<QList<double> > xLyMzM;
        QList<QList<double> > xMyMzM;
        for (int i = 0; i< cloud[0].size();i++) {
            QList<double> line;
            for (int j=0;j<cloud.size();j++)
                line.append(cloud[j][i]);
            if (cloud[0][i] <= center.x) {
                if (cloud[1][i] <= center.y) {
                    if (cloud[2][i] <= center.z)
                        xLyLzL.append(line);
                    else
                        xLyLzM.append(line);
                }
                else {
                    if (cloud[2][i] <= center.z)
                        xLyMzL.append(line);
                    else
                        xLyMzM.append(line);
                }
            }
            else {
                if (cloud[1][i] <= center.y) {
                    if (cloud[2][i] <= center.z)
                        xMyLzL.append(line);
                    else
                        xMyLzM.append(line);
                }
                else {
                    if (cloud[2][i] <= center.z)
                        xMyMzL.append(line);
                    else
                        xMyMzM.append(line);
                }
            }
        }
        result.append(xLyLzL);
        result.append(xLyMzL);
        result.append(xMyLzL);
        result.append(xMyMzL);
        result.append(xLyLzM);
        result.append(xLyMzM);
        result.append(xMyLzM);
        result.append(xMyMzM);
    }

    return result;
}

Point3D ComponentAnalyzer::getCloudCenter(QList<QList<double> > cloud) {
    Point3D p = {0.0f, 0.0f, 0.0f};

    int dimentions = cloud.size();
    if (dimentions > 3)
        dimentions = 3;

    QList<double> xData = cloud[0];
    QList<double> yData;
    QList<double> zData;
    if (dimentions > 1)
         yData = cloud[1];
    if (dimentions > 2)
         zData = cloud[2];

    double xMin = minElement(xData);
    double xMax = maxElement(xData);
    p.x = getCloudCenterByCoord(xMin, xMax);
    if (dimentions>1) {
        double yMin = minElement(yData);
        double yMax = maxElement(yData);
        p.y = getCloudCenterByCoord(yMin, yMax);
    }
    if (dimentions>2) {
        double zMin = minElement(zData);
        double zMax = maxElement(zData);
        p.z = getCloudCenterByCoord(zMin, zMax);
    }

    return p;
}

Point3D ComponentAnalyzer::getPointInCloud(QList<QList<double> > cloud) {
    QList<Vector> vectors;
    Point3D center = getCloudCenter(cloud);
    for (int i=0;i<cloud[0].size();i++) {
        Point3D temp = {cloud[0][i], 0.0f, 0.0f};
        if (cloud.size() > 1)
            temp.y = cloud[1][i];
        if (cloud.size() > 2)
            temp.z = cloud[2][i];
        vectors.append(Vector::Create(center, temp));
    }

    Vector sumVector = Vector::Create(center, center);
    for (int i=0;i<vectors.size();i++)
        sumVector = sumVector.sum(vectors[i]);

    double step = sumVector.length()/vectors.size();

    for(int i=0;i<graphNodePoints.size();i++) {
        graphNodePoints[i] = sumVector.movePoint(graphNodePoints[i], step);
    }

    return sumVector.movePoint(center, step);
}

QList<QList<double> > ComponentAnalyzer::getMaxCloudFromSectors(QList<QList<QList<double> > > sectors) {
    int maxIdx = 0;
    int maxVaule = 0;
    for (int i=0;i<sectors.size();i++) {
        //if (sectors[i].size())
        if (sectors[i].size() > maxVaule) {
            maxVaule = sectors[i].size();
            maxIdx = i;
        }
    }
    QList<QList<double> > mSector = sectors[maxIdx];

    QList<QList<double> > result;
    for (int col=0;col<mSector[0].size();col++) {
        QList<double> column;
        for (int row=0;row<mSector.size();row++) {
            column.append(mSector[row][col]);
        }
        result.append(column);
    }

    return result;
}
