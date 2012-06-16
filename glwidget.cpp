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
    scale = -6.0f;

    showGraphic = true;
    showCoords = true;

    dimentions = 2;
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

    glTranslatef(0.0,0.0f,scale);

    glRotatef(xRot,1.0f,0.0f,0.0f);
    glRotatef(yRot,0.0f,1.0f,0.0f);
    glRotatef(zRot,0.0f,0.0f,1.0f);

    if (showCoords)
        drawCoords();
    if (showGraphic)
        drawGraphic();
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

void GLWidget::wheelEvent(QWheelEvent * event) {
    scale += event->delta()*0.005f;
    updateGL();
}

void GLWidget::SetCoordsData(Point3D coords,
                             QList<QString> coordLabels,
                             Figure3D::GColor coordColor,
                             Figure3D::GColor coordsLabelsColor) {
    this->coords = coords;
    this->coordsLabels = coordLabels;
    this->coordsColor = coordColor;
    this->coordsLabelsColor = coordsLabelsColor;
}

void GLWidget::drawCoords() {
    QFont* font = new QFont("Times", 14, QFont::Normal);
    Line3D xCoord;
    xCoord.SetCurrentColor(coordsColor);
    GLfloat xLen = coords.x;
    Point3D xFrom;
    xFrom.x = -xLen;
    Point3D xTo;
    xTo.x = xLen;
    xCoord.setPoints(xFrom, xTo);
    xCoord.draw();
    Figure3D::SetNewColor(coordsLabelsColor);
    renderText(xLen, 0.0f,0.0f,coordsLabels[0], *font);

    Line3D yCoord;
    yCoord.SetCurrentColor(coordsColor);
    GLfloat yLen = coords.y;
    Point3D yFrom;
    yFrom.y = -yLen;
    Point3D yTo;
    yTo.y = yLen;
    yCoord.setPoints(yFrom, yTo);
    yCoord.draw();
    Figure3D::SetNewColor(coordsLabelsColor);
    renderText(0.0f,yLen, 0.0f,coordsLabels[1], *font);

    GLfloat zLen = coords.z;
    if (dimentions == 3) {
        Line3D zCoord;
        zCoord.SetCurrentColor(coordsColor);
        Point3D zFrom;
        zFrom.z = -zLen;
        Point3D zTo;
        zTo.z = zLen;
        zCoord.setPoints(zFrom, zTo);
        zCoord.draw();
        Figure3D::SetNewColor(coordsLabelsColor);
        renderText(0.0f, 0.0f,zLen, coordsLabels[2], *font);
    }

    GLfloat tickSize = 0.04f;
    SnowFlake3D tick;
    tick.SetCurrentColor(coordsColor);
    for (int i=(int)-xLen;i<xLen;i++) {
        Point3D tickCenter;
        tickCenter.x = i;
        tick.SetGeometry(tickCenter, tickSize);
        tick.draw();
        renderText(i, 0.0f,0.0f, QString::number(i), *font);
    }
    for (int i=(int)-yLen;i<yLen;i++) {
        Point3D tickCenter;
        tickCenter.y = i;
        tick.SetGeometry(tickCenter, tickSize);
        tick.draw();
        renderText(0.0f,i, 0.0f, QString::number(i), *font);
    }
    if (dimentions == 3) {
        for (int i=(int)-zLen;i<zLen;i++) {
            Point3D tickCenter;
            tickCenter.z = i;
            tick.SetGeometry(tickCenter, tickSize);
            tick.draw();
            renderText(0.0f,0.0f, i, QString::number(i), *font);
        }
    }
}

void GLWidget::drawGraphic() {
}



