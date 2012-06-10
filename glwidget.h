#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <GL/glu.h>
#include <QPair>

class GLWidget:public QGLWidget
{
  Q_OBJECT

    public :
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setCoords(QList<QList<QPair<QString,double> > > coords);
    void setCoordsLabels(QList<QString> labels);

    void updateShowFlags(bool showData, bool showCoords, bool showGraphic);

public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);

signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);



private:
     //QtLogo *logo;
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
     QColor qtGreen;
     QColor qtPurple;

     QList<QList<QPair<QString,double> > > points;
     QList<QString> labels;

     void drawGraphic();

     void drawCoords();

     void drawData();

     bool showGraphic;
     bool showCoords;
     bool showData;
};


#endif // GLWIDGET_H
