#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "baseanalyzer.h"
#include "statisticanalyzer.h"
#include <QMessageBox>

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

void MainWindow::testSignal(QAction* menuAction) {
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
