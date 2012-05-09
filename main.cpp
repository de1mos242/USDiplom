#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("ANSI"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
