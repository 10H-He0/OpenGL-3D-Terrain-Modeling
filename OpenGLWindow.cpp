#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

OpenGLWindow::~OpenGLWindow()
{}

void OpenGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
}

void OpenGLWindow::paintGL()
{
    makeCurrent();
    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
