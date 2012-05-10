#include "statisticanalyzedialog.h"
#include "ui_statisticanalyzedialog.h"

StatisticAnalyzeDialog::StatisticAnalyzeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticAnalyzeDialog)
{
    ui->setupUi(this);
}

StatisticAnalyzeDialog::~StatisticAnalyzeDialog()
{
    delete ui;
}
