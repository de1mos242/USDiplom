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
#include "statisticanalyzedialog.h"

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

    GraphNode* temp = new GraphNode();
    QList<QList<double> > workSector = iTMatrix;

    getCellPoints(workSector, 10);
    convertCould(iTMatrix);

    for(int i=0;i<iterationCount;i++) {
        shiftPoints();
        mergePoints();
    }

    normalizeRaduises();
    GraphNode *pNode = temp;
    for(int i=0;i<graphNodePoints.size();i++) {
        pNode->coords = graphNodePoints[i].first;
        pNode->radius = graphNodePoints[i].second;
        if (i < graphNodePoints.size()-1) {
            GraphNode *cNode = new GraphNode();
            pNode->nodes.append(cNode);
            pNode = cNode;
        }
    }


    return temp;
}

QList<QList<QList<double> > > ComponentAnalyzer::getSectors(QList<QList<double> > cloud, int divide) {
    QList<QList<QList<double> > > result;

    double xMin = minElement(cloud[0]);
    double xMax = maxElement(cloud[0]);
    double dx = (xMax - xMin)/divide;
    double yMin, yMax, zMin, zMax, dy, dz;
    if (cloud.size() > 1) {
        yMin = minElement(cloud[1]);
        yMax = maxElement(cloud[1]);
        dy = (yMax - yMin)/divide;
    }
    if (cloud.size() > 2) {
        zMin = minElement(cloud[2]);
        zMax = maxElement(cloud[2]);
        dz = (zMax - zMin)/divide;
    }

    if (cloud.size() == 1) {
        for (int i=0;i<divide;i++) {
            QList<QList<double> > temp;
            for(int row=0;row<cloud[0].size();row++) {
                if (cloud[0][row] >= xMin + i*dx && cloud[0][row] <= xMin + (i+1)*dx) {
                    QList<double> line;
                    line.append(cloud[0][row]);
                    temp.append(line);
                }

            }
            result.append(temp);
        }
    }
    else if (cloud.size() == 2) {
        for (int ix=0;ix<divide;ix++)
            for (int iy=0;iy<divide;iy++) {
                QList<QList<double> > temp;
                for(int row=0;row<cloud[0].size();row++)
                    if (cloud[0][row] >= xMin + ix*dx
                            && cloud[0][row] <= xMin + (ix+1)*dx
                            && cloud[1][row] >= yMin + iy*dy
                            && cloud[1][row] <= yMin + (iy+1)*dy) {
                        QList<double> line;
                        line.append(cloud[0][row]);
                        line.append(cloud[1][row]);
                        temp.append(line);
                    }
                result.append(temp);
            }
    }
    else if (cloud.size() > 2) {
        for (int ix=0;ix<divide;ix++)
            for (int iy=0;iy<divide;iy++)
                for (int iz=0;iz<divide;iz++) {
                    QList<QList<double> > temp;
                    for(int row=0;row<cloud[0].size();row++)
                        if (cloud[0][row] >= xMin + ix*dx
                                && cloud[0][row] <= xMin + (ix+1)*dx
                                && cloud[1][row] >= yMin + iy*dy
                                && cloud[1][row] <= yMin + (iy+1)*dy
                                && cloud[2][row] >= zMin * iz*dz
                                && cloud[2][row] <= zMin + (iz+1)*dz) {
                            QList<double> line;
                            line.append(cloud[0][row]);
                            line.append(cloud[1][row]);
                            line.append(cloud[2][row]);
                            temp.append(line);
                        }
                    result.append(temp);
                }
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
        graphNodePoints[i].first = sumVector.movePoint(graphNodePoints[i].first, step);
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

void ComponentAnalyzer::getCellPoints(QList<QList<double> > cloud, int divide) {

    double xMin = minElement(cloud[0]);
    double xMax = maxElement(cloud[0]);
    double dx = (xMax - xMin)/divide;
    double yMin = 0, yMax = 0, zMin = 0, zMax = 0, dy = 0, dz = 0;
    if (cloud.size() > 1) {
        yMin = minElement(cloud[1]);
        yMax = maxElement(cloud[1]);
        dy = (yMax - yMin)/divide;
    }
    if (cloud.size() > 2) {
        zMin = minElement(cloud[2]);
        zMax = maxElement(cloud[2]);
        dz = (zMax - zMin)/divide;
    }
    double radius = (dx/2 + dy/2 + dz/2)/3;

    if (cloud.size() == 1) {
        for (int ix=0;ix<divide;ix++) {
            double xc = ((xMin + ix*dx)+(xMin + (ix+1)*dx))/2;
            Point3D temp = {xc,0.0f,0.0f};
            graphNodePoints.append(QPair<Point3D,double>(temp, radius));
        }
    }
    else if (cloud.size() == 2) {
        for (int ix=0;ix<divide;ix++) {
            double xc = ((xMin + ix*dx)+(xMin + (ix+1)*dx))/2;
            for (int iy=0;iy<divide;iy++) {

                double yc = ((yMin + iy*dy)+(yMin + (iy+1)*dy))/2;
                Point3D temp = {xc,yc,0.0f};
                graphNodePoints.append(QPair<Point3D,double>(temp, radius));
            }
        }
    }
    else if (cloud.size() > 2) {
        for (int ix=0;ix<divide;ix++) {
            double xc = ((xMin + ix*dx)+(xMin + (ix+1)*dx))/2;
            for (int iy=0;iy<divide;iy++) {
                double yc = ((yMin + iy*dy)+(yMin + (iy+1)*dy))/2;
                for (int iz=0;iz<divide;iz++) {
                    double zc = ((zMin + iz*dz)+(zMin + (iz+1)*dz))/2;
                    Point3D temp = {xc,yc,zc};
                    graphNodePoints.append(QPair<Point3D,double>(temp, radius));
                }
            }
        }
    }
}

void ComponentAnalyzer::shiftPoints() {
    for (int idx=0;idx<graphNodePoints.size();idx++) {
        double distance = graphNodePoints[idx].second;
        Point3D point = graphNodePoints[idx].first;

        QList<Point3D> workPoints;
        foreach(Point3D cloudNode, cloud) {
            double dx = (point.x-cloudNode.x)*(point.x-cloudNode.x);
            double dy = (point.y-cloudNode.y)*(point.y-cloudNode.y);
            double dz = (point.z-cloudNode.z)*(point.z-cloudNode.z);

            if (sqrt(dx+dy+dz) <= distance)
                workPoints.append(cloudNode);
        }

        if (workPoints.size() == 0) {
            graphNodePoints.removeAt(idx);
            idx--;
            continue;
        }

        QList<Vector> vectors;
        foreach(Point3D workPoint, workPoints)
            vectors.append(Vector::Create(point, workPoint));

        Vector sumVector = Vector::Create(point, point);
        for (int i=0;i<vectors.size();i++)
            sumVector = sumVector.sum(vectors[i]);

        double step = (sumVector.length()/vectors.size()) * shiftK;

        graphNodePoints[idx].first = sumVector.movePoint(point, step);
    }
}

void ComponentAnalyzer::mergePoints() {
    for (int i = 0; i < graphNodePoints.size();i++) {
        for (int j=i+1;j < graphNodePoints.size();j++) {
            Point3D p1 = graphNodePoints[i].first;
            Point3D p2 = graphNodePoints[j].first;
            double r1 = graphNodePoints[i].second;
            double r2 = graphNodePoints[j].second;
            double dx = (p1.x-p2.x)*(p1.x-p2.x);
            double dy = (p1.y-p2.y)*(p1.y-p2.y);
            double dz = (p1.z-p2.z)*(p1.z-p2.z);

            if (sqrt(dx+dy+dz) >= (r1 + r2) * mergeK)
                continue;

            double cx = (p1.x + p2.x)/2;
            double cy = (p1.y + p2.y)/2;
            double cz = (p1.z + p2.z)/2;
            Point3D resultPoint = {cx, cy, cz};
            double newRadius = r1 + r2;
            graphNodePoints.removeAt(j);
            j--;
            graphNodePoints[i].first = resultPoint;
            graphNodePoints[i].second = newRadius;
        }
    }
}

void ComponentAnalyzer::convertCould(QList<QList<double> > cloud) {
    for (int i=0;i<cloud[0].size();i++) {
        if (cloud.size() == 1) {
            Point3D temp = {cloud[0][i], 0.0f, 0.0f};
            this->cloud.append(temp);
        }
        else if (cloud.size() == 2) {
            Point3D temp = {cloud[0][i],cloud[1][i],0.0f};
            this->cloud.append(temp);
        }
        else if (cloud.size() > 2) {
            Point3D temp = {cloud[0][i],cloud[1][i],cloud[2][i]};
            this->cloud.append(temp);
        }
    }
}


void ComponentAnalyzer::normalizeRaduises() {
    double etalonR = 0.03f;
    double minRadius = 100500.0f;
    for(int i=0;i<graphNodePoints.size();i++)
        if (graphNodePoints[i].second < minRadius)
            minRadius = graphNodePoints[i].second;
    for(int i=0;i<graphNodePoints.size();i++)
        graphNodePoints[i].second = (graphNodePoints[i].second/minRadius)*etalonR;
}

bool ComponentAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    QHashIterator<QString,QString> iterator(this->getAllParams());
    while (iterator.hasNext()) {
        iterator.next();
        dialog->addAviabledParam(iterator.value(), iterator.key());
    }
    if (!ShowGraph) {
        dialog->hideAdditions();
    }
    dialog->exec();
    if (dialog->ParametersList->count()==0) {
        return false;
    }
    parametersList = dialog->ParametersList;
    mergeK = dialog->mergeK;
    shiftK = dialog->shiftK;
    iterationCount = dialog->iterationCount;

    return true;
}
