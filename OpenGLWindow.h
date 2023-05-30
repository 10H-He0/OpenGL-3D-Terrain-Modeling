
#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QStandardPaths>
#include <QDebug>

#define PI 3.1415926535

class OpenGLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWindow(QWidget *parent = nullptr);
    ~OpenGLWindow();
    GLfloat xtrans=0, ytrans=0, ztrans=0;
    GLfloat xrot = 0, yrot=0, zrot = 0;

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *enent) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QOpenGLShaderProgram *program;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    unsigned int handle[3];

    QVector3D cameraPos   = QVector3D(6.0f, 1.0f, 0.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp    = QVector3D(0.0f, 1.0f, 0.0f);
    float cameraSpeed = static_cast<float>(2.5f * 0.1f);

    QVector2D mousePos;
    QQuaternion rotation;

    QTimer *Timer;
};
#endif // OPENGLWINDOW_H
