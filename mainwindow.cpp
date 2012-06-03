#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "baseanalyzer.h"
#include "statisticanalyzer.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QLabel>
#include "analyzerstrategy.h"
#include "componentanalyzer.h"
#include "linearcorrelationanalyzer.h"
#include "spearmananalizer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->menuANALIS->hide();
    tables = new QHash <int,QTableWidget*>();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MenuAction(QAction * menuAction) {
    if (menuAction->objectName().contains("Analyze")) {
        AnalyzeAction(menuAction);
    }
    else {
        FileAction(menuAction);
    }
}

void MainWindow::AnalyzeAction(QAction* menuAction) {
    BaseAnalyzer* analyzer;
    QString action = menuAction->objectName();
    if (action == "StatisticAnalyze") {
        analyzer = new StatisticAnalyzer();
    }
    else if (action == "ComponentAnalyze") {
        analyzer = new ComponentAnalyzer();
        analyzer->AdditionalWidgets.append(new QWidget());
        analyzer->AdditionalWidgets.append(new QWidget());
    }
    else if (action == "LinearAnalyze") {
        analyzer = new LinearCorrelationAnalyzer();
    }
    else if (action == "SpearmanAnalyze") {
        analyzer = new SpearmanAnalizer();
    }
    else {
        QMessageBox msgBox;
        msgBox.setInformativeText(menuAction->text() + " еще не реализован");
        msgBox.setWindowTitle("Menu say's:");
        msgBox.exec();
        return;
    }

    QWidget *newTab = new QWidget();
    QTableWidget *table = new QTableWidget(newTab);
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    AnalyzerStrategy *strategy = new AnalyzerStrategy();
    strategy->analyzer = analyzer;
    strategy->sourcetable = tables->value(ui->tabWidget->currentIndex());
    strategy->resultTable = table;
    if (strategy->Run()) {
        newTab->setLayout(new QGridLayout(this));
        newTab->layout()->addWidget(table);
        table->resizeColumnsToContents();
        ui->tabWidget->addTab(newTab, menuAction->text() + table->objectName());
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

        foreach(QWidget* widget, analyzer->AdditionalWidgets) {
            ui->tabWidget->addTab(widget, widget->objectName());
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        }
    }
    //delete analyzer;
}

void MainWindow::ChangeTab(int newIdx) {
    if (tables->contains(newIdx)) {
      //  ui->menuANALIS->show();
    }
    else {
       // ui->menuANALIS->hide();
    }
}

void MainWindow::FileAction(QAction * fileAction) {
    if (fileAction->objectName() == "OpenFile") {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.csv)"));
        if (fileName != "") {
            showOpenedFile(fileName);
        }

    }
    else if (fileAction->objectName() == "Exit"){
        this->close();
    }
}

void MainWindow::showOpenedFile(QString filename) {
    QWidget *tab = new QWidget (this);
    QFileInfo fi(filename);
    ui->tabWidget->addTab(tab, fi.fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

    QTableWidget *table = new QTableWidget(tab);
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    QFile *file = new QFile(filename);
    file->open(QIODevice::ReadOnly|QIODevice::Text);
    QString header = file->readLine();
    header = header.replace("\n","");
    QString separator = ";";
    QStringList headerList = header.split(separator);
    if (headerList.count() <= 1) {
        separator = ",";
        headerList = header.split(separator);
    }
    int columnsCount = headerList.count();
    table->setColumnCount(columnsCount);
    for (int i=0;i<columnsCount;i++)
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList[i]));
    for (int i=0;!file->atEnd(); i++) {
        QString line = file->readLine();
        line = line.replace("\n","");
        QStringList lineList = line.split(separator);
        table->setRowCount(i+1);
        for (int j=0;j<columnsCount;j++) {
            if (j < lineList.count())
                table->setItem(i,j,new QTableWidgetItem(lineList[j]));
        }
    }

    tab->setLayout(new QGridLayout(this));
    tab->layout()->addWidget(table);
    table->resizeColumnsToContents();

    tables->insert(ui->tabWidget->currentIndex(), table);
}
