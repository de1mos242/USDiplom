#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <GL/glu.h>
#include <QPair>
#include <figure3d.h>

class GLWidget:public QGLWidget
{
  Q_OBJECT

    public :
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void SetCoordsData(Point3D coords,
                       QList<QString> coordLabels,
                       Figure3D::GColor coordColor = Figure3D::black,
                       Figure3D::GColor coordsLabelsColor = Figure3D::darkGreen);

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
    void wheelEvent(QWheelEvent *event);
    virtual void drawGraphic();



    int dimentions;

private:
     int xRot;
     int yRot;
     int zRot;
     double scale;
     QPoint lastPos;

     bool showGraphic;
     bool showCoords;

     Point3D coords;
     QList<QString> coordsLabels;
     Figure3D::GColor coordsLabelsColor;
     Figure3D::GColor coordsColor;

     void drawCoords();
};


#endif // GLWIDGET_H
