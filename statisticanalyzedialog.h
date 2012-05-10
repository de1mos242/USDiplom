#ifndef STATISTICANALYZEDIALOG_H
#define STATISTICANALYZEDIALOG_H

#include <QDialog>

namespace Ui {
class StatisticAnalyzeDialog;
}

class StatisticAnalyzeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StatisticAnalyzeDialog(QWidget *parent = 0);
    ~StatisticAnalyzeDialog();
    
private:
    Ui::StatisticAnalyzeDialog *ui;
};

#endif // STATISTICANALYZEDIALOG_H
