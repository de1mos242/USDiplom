#include "statisticanalyzedialog.h"
#include "ui_statisticanalyzedialog.h"
#include <QMessageBox>

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

void StatisticAnalyzeDialog::addAviabledParam(QString name, QString value) {
    QListWidgetItem *item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, value);
    moveParam(ui->selectedParams, ui->aviabledParams, item);
}

void StatisticAnalyzeDialog::moveParam(QListWidget *from, QListWidget *to, QListWidgetItem *what) {
    int row = from->row(what);
    if (row >= 0)
        from->removeItemWidget(from->takeItem(row));
    to->addItem(what);
    to->sortItems();
}

void StatisticAnalyzeDialog::deleteParam() {
    QList <QListWidgetItem *> selectedItems = ui->selectedParams->selectedItems();
    if (selectedItems.count()>0)
        moveParam(ui->selectedParams, ui->aviabledParams,selectedItems.at(0));
}

void StatisticAnalyzeDialog::addParam() {
    QList <QListWidgetItem *> selectedItems = ui->aviabledParams->selectedItems();
    if (selectedItems.count()>0)
        moveParam(ui->aviabledParams, ui->selectedParams,selectedItems.at(0));
}

void StatisticAnalyzeDialog::run() {
    ParametersList = new QList <QString>();
    for (int i=0;i<ui->selectedParams->count();i++) {
        ParametersList->append(ui->selectedParams->item(i)->data(Qt::UserRole).toString());
    }
    if (ParametersList->count() == 0) {
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("Выберите параметры");
        msg->exec();
        return;
    }
    this->close();
}

void StatisticAnalyzeDialog::cancel() {
    this->close();
}
