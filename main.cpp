#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
