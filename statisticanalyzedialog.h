#ifndef STATISTICANALYZEDIALOG_H
#define STATISTICANALYZEDIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class StatisticAnalyzeDialog;
}

class StatisticAnalyzeDialog : public QDialog
{
    Q_OBJECT
    
public:
    QList <QString> *ParametersList;

    explicit StatisticAnalyzeDialog(QWidget *parent = 0);
    ~StatisticAnalyzeDialog();
    void addAviabledParam(QString name, QString value);
    void addAdditionalParam(QWidget *widget, QString name);
    int GetIntParam(QString name);
    double GetDoubleParam(QString name);
    QString GetStringParam(QString name);
    void SetDialogName(QString name);

public slots:
    void addParam();
    void deleteParam();
    void run();
    void cancel();
    
private:
    Ui::StatisticAnalyzeDialog *ui;
    void moveParam(QListWidget *from, QListWidget *to, QListWidgetItem *what, bool needSort);

    QHash<QString,QWidget*> additionalWidgets;
};

#endif // STATISTICANALYZEDIALOG_H
