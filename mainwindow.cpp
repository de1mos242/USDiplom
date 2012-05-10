#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "baseanalyzer.h"
#include "statisticanalyzer.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    if (menuAction->objectName() == "StatisticAnalyze") {
        analyzer = new StatisticAnalyzer();
    }
    else {
        QMessageBox msgBox;
        msgBox.setInformativeText(menuAction->text() + " еще не реализован");
        msgBox.setWindowTitle("Menu say's:");
        msgBox.exec();
        return;
    }

    analyzer->DoAnalyze();
    delete analyzer;
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
}
