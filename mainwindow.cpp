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
#include <QTextStream>
#include <QTextCodec>
#include <QPushButton>
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->menuANALIS->hide();
    tables = new QHash <int,QTableWidget*>();
    connect(ui->openNewFileButton, SIGNAL(clicked()), this, SLOT(openNewFile()));
    ui->tabWidget->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MenuAction(QAction * menuAction) {
    if (menuAction->objectName().contains("Analyze")) {
        AnalyzeAction(menuAction);
    }
    else if (menuAction->objectName().contains("File")){
        FileAction(menuAction);
    }
    else if (menuAction->objectName().contains("Graphic")) {
        updateGraphics();
    }
}

void MainWindow::updateGraphics() {
    foreach (QWidget* graphicWidget, graphicWidgets) {
        GLWidget *gl = (GLWidget*)graphicWidget->children().at(0);
        gl->updateShowFlags(
                    ui->menuGraphicShowData->isChecked(),
                    ui->menuGraphicShowCoords->isChecked(),
                    ui->menuGraphicShowGraphic->isChecked());
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
    }
    else if (action == "LinearAnalyze") {
        analyzer = new LinearCorrelationAnalyzer();
    }
    else if (action == "SpearmanAnalyze") {
        analyzer = new SpearmanAnalizer();
    }
    else if (action == "TopologicAnalyze") {
        analyzer = new ComponentAnalyzer();
        analyzer->AdditionalWidgets.append(new QWidget());
        ((ComponentAnalyzer*)analyzer)->ShowGraph = true;
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
        if (table->objectName() != "") {
            ui->tabWidget->addTab(newTab, menuAction->text());
        }
        else {
            ui->tabWidget->addTab(newTab, table->objectName());
        }
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

        foreach(QWidget* widget, analyzer->AdditionalWidgets) {
            ui->tabWidget->addTab(widget, widget->objectName());
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
            if (widget->objectName().contains(tr("График"))) {
                graphicWidgets.append(widget);
                updateGraphics();
            }
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
        openNewFile();
    }
    else if (fileAction->objectName() == "Exit"){
        this->close();
    }
}

void MainWindow::openNewFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.csv)"));
    if (fileName != "") {
        ui->tabWidget->setHidden(false);
        ui->openNewFileButton->setHidden(true);
        showOpenedFile(fileName);
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
    QTextStream stream(file);
    stream.setCodec(QTextCodec::codecForName("CP1251"));
    //file->setEncodingFunction();

    QString header = stream.readLine();
    header = header.replace("\n","");
    QString separator = ";";
    QStringList headerList = header.split(separator);
    if (headerList.count() <= 1) {
        separator = ",";
        headerList = header.split(separator);
    }
    int columnsCount = headerList.count();
    table->setColumnCount(columnsCount-1);
    for (int i=0;i<columnsCount-1;i++)
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(headerList[i+1]));
    for (int i=0;!stream.atEnd(); i++) {
        QString line = stream.readLine();
        line = line.replace("\n","");
        QStringList lineList = line.split(separator);
        table->setRowCount(i+1);
        table->setVerticalHeaderItem(i, new QTableWidgetItem(lineList[0]));
        for (int j=1;j<columnsCount;j++) {
            if (j < lineList.count())
                table->setItem(i,j-1,new QTableWidgetItem(lineList[j]));
        }
    }

    tab->setLayout(new QGridLayout(this));
    tab->layout()->addWidget(table);
    table->resizeColumnsToContents();

    tables->insert(ui->tabWidget->currentIndex(), table);
}
