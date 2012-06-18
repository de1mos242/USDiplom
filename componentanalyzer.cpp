#include "componentanalyzer.h"
#include "QTableWidgetItem"
#include <QTableWidget>
#include <QHeaderView>
#include <QMatrix>
#include <qmath.h>
#include <QGridLayout>
#include "graphic2d.h"

ComponentAnalyzer::ComponentAnalyzer()
{
    e1 = 0.5;
    e2 = 0.4;
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
    table->setColumnCount(parametersCount);
    table->setRowCount(parametersCount);
    table->setObjectName(" P Матрица");
    for (int i=0;i<parametersCount;i++) {
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
        table->setVerticalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<parametersCount;i++)
        for(int j=0;j<parametersCount;j++)
            table->setItem(i,j,new QTableWidgetItem(QString::number(PMatrix[i][j])));


    QWidget* tab = AdditionalWidgets.at(0);
    tab->setObjectName("T Матрица");
    QTableWidget* Ttable = new QTableWidget();
    Ttable->setEditTriggers(QTableWidget::NoEditTriggers);

    Ttable->setColumnCount(parametersCount);
    Ttable->setRowCount(TMatrix.count());

    for (int i=0;i<parametersCount;i++) {
        Ttable->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<TMatrix.count();i++)
        for(int j=0;j<parametersCount;j++)
            Ttable->setItem(i,j,new QTableWidgetItem(QString::number(TMatrix[i][j])));

    tab->setLayout(new QGridLayout());
    Ttable->setObjectName("ttable");
    tab->layout()->addWidget(Ttable);
    Ttable->resizeColumnsToContents();

    Point3D coords;
    coords.x = 5.0f;
    coords.y = 5.0f;
    QList<QString> coordLables;
    coordLables.append("PC1");
    coordLables.append("PC2");

    QWidget * tab2 = AdditionalWidgets.at(1);
    tab2->setObjectName("T1");
    Graphic2D * g1 = new Graphic2D(tab2);
    //g1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    coordLables.append(headerList.at(parametersList->at(0).toInt()));
    coordLables.append(headerList.at(parametersList->at(1).toInt()));
    g1->SetCoordsData(coords, coordLables);
    g1->SetPoints(getTmatrix1());
    tab2->setLayout(new QGridLayout());
    tab2->layout()->addWidget(g1);

    coordLables.clear();
    coordLables.append("PC3");
    coordLables.append("PC4");

    QWidget * tab3 = AdditionalWidgets.at(2);
    tab3->setObjectName("T2");
    Graphic2D * g2 = new Graphic2D(tab3);
    //g2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    coordLables.clear();
    coordLables.append(headerList.at(parametersList->at(2).toInt()));
    coordLables.append(headerList.at(parametersList->at(3).toInt()));
    g2->SetCoordsData(coords, coordLables);
    g2->SetPoints(getTmatrix2());
    tab3->setLayout(new QGridLayout());
    tab3->layout()->addWidget(g2);

    coordLables.clear();
    coordLables.append("PC1");
    coordLables.append("PC2");

    QWidget * tab4 = AdditionalWidgets.at(3);
    tab4->setObjectName("P1");
    Graphic2D * g3 = new Graphic2D(tab4);
    //g3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    /*coordLables.clear();
    coordLables.append(headerList.at(parametersList->at(0).toInt()));
    coordLables.append(headerList.at(parametersList->at(1).toInt()));*/
    g3->SetCoordsData(coords, coordLables);
    g3->SetPoints(getPmatrix1());
    tab4->setLayout(new QGridLayout());
    tab4->layout()->addWidget(g3);

    coordLables.clear();
    coordLables.append("PC3");
    coordLables.append("PC4");

    QWidget * tab5 = AdditionalWidgets.at(4);
    tab5->setObjectName("P2");
    Graphic2D * g4 = new Graphic2D(tab5);
    //g4->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    /*coordLables.clear();
    coordLables.append(headerList.at(parametersList->at(2).toInt()));
    coordLables.append(headerList.at(parametersList->at(3).toInt()));*/
    g4->SetCoordsData(coords, coordLables);
    g4->SetPoints(getPmatrix2());
    tab5->setLayout(new QGridLayout());
    tab5->layout()->addWidget(g4);

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

QList<Point3DEx> ComponentAnalyzer::getTmatrix1() {
    QList<Point3DEx> result;
    for (int i=0;i<TMatrix.count();i++) {
        Point3DEx p;
        p.x = TMatrix[i][0];
        p.y = TMatrix[i][1];
        p.label = QString::number(i+1);
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}

QList<Point3DEx> ComponentAnalyzer::getTmatrix2() {
    QList<Point3DEx> result;
    for (int i=0;i<TMatrix.count();i++) {
        Point3DEx p;
        p.x = TMatrix[i][2];
        p.y = TMatrix[i][3];
        p.label = QString::number(i+1);
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}

QList<Point3DEx> ComponentAnalyzer::getPmatrix1() {
    QList<Point3DEx> result;
    for (int i=0;i<PMatrix.count();i++) {
        Point3DEx p;
        p.x = PMatrix[i][0];
        p.y = PMatrix[i][1];
        p.label = headerList.at(parametersList->at(i).toInt());
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}

QList<Point3DEx> ComponentAnalyzer::getPmatrix2() {
    QList<Point3DEx> result;
    for (int i=0;i<PMatrix.count();i++) {
        Point3DEx p;
        p.x = PMatrix[i][2];
        p.y = PMatrix[i][3];
        p.label = headerList.at(parametersList->at(i).toInt());
        p.color = Figure3D::red;
        result.append(p);
    }
    return result;
}
