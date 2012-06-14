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

public slots:
    void addParam();
    void deleteParam();
    void run();
    void cancel();
    
private:
    Ui::StatisticAnalyzeDialog *ui;
    void moveParam(QListWidget *from, QListWidget *to, QListWidgetItem *what, bool needSort);
};

#endif // STATISTICANALYZEDIALOG_H
