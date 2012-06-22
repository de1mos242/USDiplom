#include "statisticanalyzedialog.h"
#include "ui_statisticanalyzedialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>

StatisticAnalyzeDialog::StatisticAnalyzeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticAnalyzeDialog)
{
    ui->setupUi(this);
    noParams = false;
}

StatisticAnalyzeDialog::~StatisticAnalyzeDialog()
{
    delete ui;
}

void StatisticAnalyzeDialog::addAviabledParam(QString name, QString value) {
    QListWidgetItem *item = new QListWidgetItem(name);
    item->setData(Qt::UserRole, value);
    moveParam(ui->selectedParams, ui->aviabledParams, item, true);
}

void StatisticAnalyzeDialog::moveParam(QListWidget *from, QListWidget *to, QListWidgetItem *what, bool needSort = false) {
    int row = from->row(what);
    if (row >= 0)
        from->removeItemWidget(from->takeItem(row));
    to->addItem(what);
    if (needSort)
        to->sortItems();
}

void StatisticAnalyzeDialog::deleteParam() {
    QList <QListWidgetItem *> selectedItems = ui->selectedParams->selectedItems();
    if (selectedItems.count()>0)
        moveParam(ui->selectedParams, ui->aviabledParams,selectedItems.at(0), true);
}

void StatisticAnalyzeDialog::addParam() {
    QList <QListWidgetItem *> selectedItems = ui->aviabledParams->selectedItems();
    if (selectedItems.count()>0)
        moveParam(ui->aviabledParams, ui->selectedParams,selectedItems.at(0));
}

void StatisticAnalyzeDialog::run() {
    ParametersList = new QList <QString>();
    if (!noParams) {
        for (int i=0;i<ui->selectedParams->count();i++) {
            ParametersList->append(ui->selectedParams->item(i)->data(Qt::UserRole).toString());
        }
        if (ParametersList->count() == 0) {
            QMessageBox *msg = new QMessageBox(this);
            msg->setText("Выберите параметры");
            msg->exec();
            return;
        }
    }
    this->close();
}

void StatisticAnalyzeDialog::cancel() {
    ParametersList = new QList<QString>();
    this->close();
}

void StatisticAnalyzeDialog::addAdditionalParam(QWidget *widget, QString name) {
    QWidget *box = new QWidget();
    box->setLayout(new QHBoxLayout());
    QLabel *label = new QLabel();
    label->setText(name);
    box->layout()->addWidget(label);
    box->layout()->addWidget(widget);
    ui->commonLayout->layout()->addWidget(box);

    additionalWidgets.insert(name, widget);
}

int StatisticAnalyzeDialog::GetIntParam(QString name) {
    QSpinBox* w = (QSpinBox*)additionalWidgets.value(name);
    return w->value();
}

double StatisticAnalyzeDialog::GetDoubleParam(QString name) {
    QDoubleSpinBox* w = (QDoubleSpinBox*)additionalWidgets.value(name);
    return w->value();
}

QString StatisticAnalyzeDialog::GetStringParam(QString name) {
    QTextEdit* w = (QTextEdit*)additionalWidgets.value(name);
    return w->toPlainText();
}

void StatisticAnalyzeDialog::SetDialogName(QString name) {
    setWindowTitle(name);
}

void StatisticAnalyzeDialog::HideParamsList() {
    QLayoutItem* item;
    while ( ( item = ui->addDeleteButtonLayout->takeAt(0)) != NULL)
    {
        delete item->widget();
        delete item;
    }
    delete ui->addDeleteButtonLayout;
    while ( ( item = ui->paramsLayout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    delete ui->paramsLayout;

    /*ui->paramsLayout->removeWidget(ui->aviabledParams);
    ui->paramsLayout->removeWidget(ui->selectedParams);
    ui->paramsLayout->removeWidget(ui->addButton);
    ui->paramsLayout->removeWidget(ui->deleteButton);
    ui->paramsLayout->removeWidget(ui->addDeleteButtonLayout->widget());
    ui->commonLayout->removeWidget(ui->paramsLayout->widget());*/
    noParams = true;
}
