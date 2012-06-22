#include "adaptivegroupanalyzer.h"
#include "statisticanalyzedialog.h"
#include <QSpinBox>
#include "componentanalyzer.h"
#include <QTableWidget>
#include "adaptivegroupanalyzegraphic.h"

AdaptiveGroupAnalyzer::AdaptiveGroupAnalyzer()
{
}

void AdaptiveGroupAnalyzer::fillInputData() {
    QList<QList<QList<double> > > tempTables;
    QList<QList<double> > tempTable;
    for (int row=0;row<rowsCount;row++) {
        QList<double> tempRow;
        tempRow.clear();
        bool skip = false;
        bool empty = false;
        for (int col=0;col<columnsCount;col++) {
            QString cell = table->item(row, col)->text();
            if (cell == "@")
                skip = true;
            if (cell == "") {
                empty = true;
                break;
            }
            tempRow.append(cell.toDouble());
        }
        if (skip) {
            continue;
        }

        if (empty) {
            tempTables.append(tempTable);
            tempTable.clear();
            continue;
        }

        tempTable.append(tempRow);
    }
    tempTables.append(tempTable);

    for (int i=0;i<tempTables.size();i++) {
        QList<QList<double> > data;
        for (int col=0;col<tempTables[i][0].size();col++) {
            QList<double> column;
            for (int row=0;row<tempTables[i].size();row++) {
                column.append(tempTables[i][row][col]);
            }
            data.append(column);
        }
        subSystemsData.append(data);
    }
}

bool AdaptiveGroupAnalyzer::showDialog() {
    fillInputTableData();
    StatisticAnalyzeDialog *dialog = new StatisticAnalyzeDialog();
    dialog->SetDialogName(name);
    dialog->addAdditionalParam(new QSpinBox(), "Количество подсистем:");
    dialog->HideParamsList();

    dialog->exec();

    int subSysmtesCount = dialog->GetIntParam("Количество подсистем:");

    for (int i=0;i<subSysmtesCount;i++) {
        StatisticAnalyzeDialog *subSystemParamsDialog = new StatisticAnalyzeDialog();
        subSystemParamsDialog->SetDialogName("Введите параметры " + QString::number(i+1) + " подсистемы:");
        QHashIterator<QString,QString> iterator(this->getAllParams());
        while (iterator.hasNext()) {
            iterator.next();
            subSystemParamsDialog->addAviabledParam(iterator.value(), iterator.key());
        }
        subSystemParamsDialog->exec();
        if (subSystemParamsDialog->ParametersList->count()==0) {
            return false;
        }
        subSystemsParams.append(*subSystemParamsDialog->ParametersList);
    }

    return true;
}

void AdaptiveGroupAnalyzer::DoAnalyze() {
    BaseAnalyzer::DoAnalyze();
    for (int i=0;i<subSystemsData.size();i++) {

        QTableWidget* tab = new QTableWidget();

        tab->setColumnCount(subSystemsData[i].size());
        tab->setRowCount(subSystemsData[i][0].size());
        for (int col=0;col<tab->columnCount();col++)
        {
            tab->setHorizontalHeaderItem(col, new QTableWidgetItem(this->table->horizontalHeaderItem(col)->text()));
            for (int row=0;row<tab->rowCount();row++)
                tab->setItem(row, col, new QTableWidgetItem(QString::number(subSystemsData[i][col][row])));
        }

        ComponentAnalyzer* component = new ComponentAnalyzer();
        component->table = tab;
        QList<QString>* paramsList = new QList<QString>();
        QHashIterator<QString,QString> iterator(this->getAllParams());
        while (iterator.hasNext()) {
            iterator.next();
            paramsList->append(iterator.key());
        }
        component->parametersList = paramsList;
        component->fillInputTableData();
        component->DoAnalyze();
        componentMatricies.append(component->PMatrix);
    }

    for (int i=0;i<subSystemsParams.size();i++) {
        QList<double> resultRow;
        for (int j=0;j<componentMatricies.size();j++) {
            QList<double> tempColumn;
            for(int row=0;row<subSystemsParams[i].size();row++) {
                tempColumn.append(componentMatricies[j][subSystemsParams[i][row].toDouble()][0]);
            }
            resultRow.append(average(tempColumn));
        }
        results.append(resultRow);
    }
}

void AdaptiveGroupAnalyzer::printResults(QTableWidget * table) {
    table->setColumnCount(subSystemsParams.size());
    table->setRowCount(componentMatricies.size());
    for (int row = 0;row<table->rowCount();row++)
        for (int col = 0;col<table->columnCount();col++)
            table->setItem(row, col, new QTableWidgetItem(QString::number(results[col][row])));

    Point3D coords;
    coords.x = 5.0f;
    coords.y = 5.0f;
    QList<QString> coordLables;
    coordLables.append("");
    coordLables.append("");

    QWidget * tab1 = AdditionalWidgets.at(0);
    tab1->setObjectName(tr("График"));
    AdaptiveGroupAnalyzeGraphic * g1 = new AdaptiveGroupAnalyzeGraphic(tab1);
    g1->SetCoordsData(coords, coordLables);
    g1->SetPoints(results);
    tab1->setLayout(new QGridLayout());
    tab1->layout()->addWidget(g1);
}
