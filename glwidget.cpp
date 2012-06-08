#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

#include <math.h>

#include "glwidget.h"
#include "snowflake3d.h"
#include "line3d.h"
#include "cylinder3d.h"
#include "sphere3d.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(((QWidget*)parent())->width(), ((QWidget*)parent())->height());
}

QSize GLWidget::sizeHint() const
{
    return QSize(((QWidget*)parent())->width(), ((QWidget*)parent())->height());
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0,-1.0f,-6.0f);

    glRotatef(xRot,1.0f,0.0f,0.0f);
    glRotatef(yRot,0.0f,1.0f,0.0f);
    glRotatef(zRot,0.0f,0.0f,1.0f);

    drawCoords();

    drawGraphic();
    /*Point3D p1 = {-0.5f, -0.5f, 0.0f};
    Point3D p2 = {-0.5f, 0.5f, 0.0f};
    Point3D p3 = {0.5f, 0.5f, 0.5f};
    Point3D p4 = {-1.0f, 1.0f, 0.5f};


    Cylinder3D cyl;
    cyl.SetCurrentColor(Cylinder3D::green);
    cyl.setGeometry(p1,p2, 0.03f);
    cyl.draw();
    cyl.setGeometry(p2,p3, 0.03f);
    cyl.draw();
    cyl.setGeometry(p2,p4, 0.03f);
    cyl.draw();

    Sphere3D sp;
    sp.SetCurrentColor(Sphere3D::red);
    sp.SetGeometry(p1, 0.05f);
    sp.draw();
    sp.SetGeometry(p2, 0.05f);
    sp.draw();
    sp.SetGeometry(p3, 0.05f);
    sp.draw();
    sp.SetGeometry(p4, 0.05f);
    sp.draw();

    SnowFlake3D flake;

    flake.SetCurrentColor(SnowFlake3D::red);
    Point3D c1 = {-1.5f, 1.0f, 0.0f};
    flake.SetGeometry(c1, 0.5f);
    flake.draw();

    flake.SetCurrentColor(SnowFlake3D::blue);
    Point3D c2 = {0.0f, -1.0f, -1.0f};
    flake.SetGeometry(c2, 0.3f);
    flake.draw();

    flake.SetCurrentColor(SnowFlake3D::green);
    Point3D c3 = {1.5f, 0.0f, 1.0f};
    flake.SetGeometry(c3, 0.5f);
    flake.draw();
    */
}

void GLWidget::resizeGL(int width, int height)
{
    if (height==0)                              // Prevent A Divide By Zero By
    {
        height=1;                           // Making Height Equal One
    }

    glViewport(0, 0, width, height);                    // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                           // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective((GLdouble)45.0f,(GLdouble)width/(GLdouble)height,(GLdouble)0.1f,(GLdouble)100.0f);
    glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
    glLoadIdentity();                           // Reset The Modelview Matrix
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    int boost = 1;

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + boost * dy);
        setYRotation(yRot + boost * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + boost * dy);
        setZRotation(zRot + boost * dx);
    }
    lastPos = event->pos();
}

void GLWidget::setCoords(QList<QList<QPair<QString, double> > > coords) {
    this->points = coords;
}

void GLWidget::drawGraphic() {
    bool hasY = (points.size()>1);
    bool hasZ = (points.size()>2);
    QList<Point3D> dots;
    for (int i=0;i<points[0].size();i++) {
        Point3D p;
        p.x = points[0][i].second;
        if (hasY)
            p.y = points[1][i].second;
        else
            p.y = 0.0f;
        if (hasZ)
            p.z = points[2][i].second;
        else
            p.z = 0.0f;
        dots.append(p);
    }

    SnowFlake3D flake;
    for (int i = 0;i<dots.size();i++) {
        flake.SetCurrentColor(SnowFlake3D::red);
        flake.SetGeometry(dots[i], 0.1f);
        flake.draw();
        //Figure3D::SetNewColor(Figure3D::green);
        //renderText(dots[i].x, dots[i].y,dots[i].z, points[0][i].first);
    }
    renderText(1.0f, 1.0f,1.0f,"Test");
}

void GLWidget::drawCoords() {
    GLfloat coordLength = 10.0f;

    Line3D xCoord;
    xCoord.SetCurrentColor(Line3D::black);
    Point3D xFrom = {-coordLength, 0.0f, 0.0f};
    Point3D xTo = {coordLength, 0.0f, 0.0f};
    xCoord.setPoints(xFrom, xTo);
    xCoord.draw();

    if (points.size()>1) {
        Line3D yCoord;
        yCoord.SetCurrentColor(Line3D::black);
        Point3D yFrom = {0.0f, -coordLength, 0.0f};
        Point3D yTo = {0.0f, coordLength, 0.0f};
        yCoord.setPoints(yFrom, yTo);
        yCoord.draw();
    }

    if (points.size()>2) {
        Line3D zCoord;
        zCoord.SetCurrentColor(Line3D::black);
        Point3D zFrom = {0.0f, 0.0f, -coordLength};
        Point3D zTo = {0.0f, 0.0f, coordLength};
        zCoord.setPoints(zFrom, zTo);
        zCoord.draw();
    }
}

