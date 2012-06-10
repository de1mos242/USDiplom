#include "componentanalyzer.h"
#include "QTableWidgetItem"
#include <QTableWidget>
#include <QHeaderView>
#include <QMatrix>
#include <qmath.h>
#include <QGridLayout>
#include "glwidget.h"

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
    table->setObjectName(tr(" P Матрица"));
    for (int i=0;i<parametersCount;i++) {
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
        table->setVerticalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<parametersCount;i++)
        for(int j=0;j<parametersCount;j++)
            table->setItem(i,j,new QTableWidgetItem(QString::number(PMatrix[i][j])));


    QWidget* tab = AdditionalWidgets.at(0);
    tab->setObjectName(tr("T Матрица"));
    QTableWidget* Ttable = new QTableWidget();
    Ttable->setEditTriggers(QTableWidget::NoEditTriggers);

    Ttable->setColumnCount(parametersCount);
    Ttable->setRowCount(TMatrix.count());

    for (int i=0;i<parametersCount;i++) {
        Ttable->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList.at(parametersList->at(i).toInt())));
    }

    for(int i=0;i<TMatrix.count();i++) {
        Ttable->setVerticalHeaderItem(i, new QTableWidgetItem(this->table->verticalHeaderItem(i)->text()));
        for(int j=0;j<parametersCount;j++)
            Ttable->setItem(i,j,new QTableWidgetItem(QString::number(TMatrix[i][j])));
    }

    tab->setLayout(new QGridLayout());
    tab->layout()->addWidget(Ttable);
    Ttable->resizeColumnsToContents();

    QWidget * tab2 = AdditionalWidgets.at(1);
    tab2->setObjectName(tr("График"));
    GLWidget *gl = new GLWidget(tab2);
    gl->setCoords(prepareGraphicData());
    QList<QString> labels;
    for (int i = 0; i<parametersCount; i++)
        labels.append(headerList.at(parametersList->at(i).toInt()));
    gl->setCoordsLabels(labels);
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
    QList<QList<double> > temp;
    for (int col=0;col<matrix[0].size();col++) {
        QList<double> column;
        for (int row=0;row<matrix.size();row++) {
            column.append(matrix[row][col]);
        }
        temp.append(column);
    }

    for (int col=0;col<temp.size();col++) {
        double avg = average(temp[col]);
        double deriviation = standardDeviation(temp[col]);
        for (int row=0;row<temp[col].size();row++) {
            double value = (temp[col][row] - avg)/deriviation;
            value = roundValue(fabs(value));
            matrix[row][col] = value;
        }
    }
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
