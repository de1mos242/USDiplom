#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void MenuAction(QAction*);
    void ChangeTab(int);
    void OnCloseTab(int tab);
    
private:
    void AnalyzeAction(QAction*);
    void FileAction(QAction*);
    Ui::MainWindow *ui;
    void showOpenedFile(QString);
    void saveOpenedTab();

    QList <QTableWidget *> *tables;
};

#endif // MAINWINDOW_H
