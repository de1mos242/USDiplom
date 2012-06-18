#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T00:52:35
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = USDiplom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    baseanalyzer.cpp \
    statisticanalyzer.cpp \
    analyzerstrategy.cpp \
    statisticanalyzedialog.cpp \
    componentanalyzer.cpp \
    linearcorrelationanalyzer.cpp \
    spearmananalizer.cpp \
    glwidget.cpp \
    figure3d.cpp \
    line3d.cpp \
    snowflake3d.cpp \
    cylinder3d.cpp \
    sphere3d.cpp \
    vector.cpp \
    graphic2d.cpp \
    elacticmapanalyzer.cpp \
    graphic3d.cpp \
    pairregressionanalyzer.cpp \
    pairregressionanalyzergraphic.cpp \
    linearregressionanalyzer.cpp \
    clusteranalyzer.cpp

HEADERS  += mainwindow.h \
    baseanalyzer.h \
    statisticanalyzer.h \
    analyzerstrategy.h \
    statisticanalyzedialog.h \
    componentanalyzer.h \
    linearcorrelationanalyzer.h \
    spearmananalizer.h \
    glwidget.h \
    figure3d.h \
    line3d.h \
    snowflake3d.h \
    cylinder3d.h \
    sphere3d.h \
    vector.h \
    graphic2d.h \
    elacticmapanalyzer.h \
    graphic3d.h \
    pairregressionanalyzer.h \
    pairregressionanalyzergraphic.h \
    linearregressionanalyzer.h \
    clusteranalyzer.h

FORMS    += mainwindow.ui \
    statisticanalyzedialog.ui

RESOURCES +=

unix:!macx:!symbian: LIBS += -lGLU
win32: LIBS += -lglu32
