#include "OpenGLWindow.h"
#include "Terrian.h"
#include "QVector3D"

Terrian terrian(0,0);

OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->grabKeyboard();
    this->setFocusPolicy(Qt::StrongFocus);
}
OpenGLWindow::~OpenGLWindow()
{}

void OpenGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();

    qDebug() << QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);

    //vshader->compileSourceFile("D:/code/OpenGL-3D-Terrain-Modeling/shader/vert.vert");
    vshader->compileSourceFile("D:/gittest/test_c/3d/OpenGL-3D-Terrain-Modeling/shader/vert.vert");
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);

    //fshader->compileSourceFile("D:/code/OpenGL-3D-Terrain-Modeling/shader/frag.frag");
    fshader->compileSourceFile("D:/gittest/test_c/3d/OpenGL-3D-Terrain-Modeling/shader/frag.frag");

    terrian.loadterrian();
    //qDebug() << "load end";
    program = new QOpenGLShaderProgram();
    program->addShader(vshader);
    program->addShader(fshader);

    program->link();
    program->bind();

    view.setToIdentity();
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glClearColor(0.5f, 0.5f, 1.0f, 1);
    glEnable(GL_DEPTH_TEST);

    xtrans=0; ytrans = 0; ztrans = 0;
    glGenBuffers(3, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terrian.afterloop_pos), terrian.afterloop_pos, GL_STATIC_DRAW);

    GLuint vPosition = program->attributeLocation("VertexPosition");
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

    model.setToIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearDepthf(1.0f);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    projection.setToIdentity();
    projection.perspective(60.0f, (GLfloat)w/(GLfloat)h, 0.001f, 100.0f);
}

void OpenGLWindow::paintGL()
{
    model.rotate(xrot, 1.0, 0.0, 0.0);
    model.rotate(yrot, 0.0, 1.0, 0.0);
    model.translate(xtrans, ytrans, ztrans);
    view.setToIdentity();
    qDebug() << cameraPos << '\n';
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    xtrans=0;
    ytrans=0;
    ztrans=0;
    xrot=0;
    yrot=0;
    zrot=0;
    program->setUniformValue("view", view);
    program->setUniformValue("projection", projection);
    program->setUniformValue("model", model);

    program->bind();

    int tmp = img_width * 2 - 1;
    for (int s=0; s<tmp-1; s++)
    {
        glDrawElements(GL_TRIANGLES, (tmp-1)*6, GL_UNSIGNED_INT, &terrian.triangles[(tmp-1)*6*s]);
    }
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
{
    mousePos = QVector2D(event->pos());
    event->accept();
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QVector2D newPos = (QVector2D)event->pos();
        QVector2D diff = newPos - mousePos;

        if (diff.x() > 0) yrot = 30.0f * PI / 180;
        else yrot = -30.0f * PI / 180;
        //        if (diff.y() > 0) xrot = 30.f * PI / 180;
        //        else xrot = -30.0f * PI / 180;
        mousePos = newPos;
        this->update();
    }
    event->accept();
}

void OpenGLWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (numDegrees.y() > 0) {
        ztrans += 0.25f;
    } else if (numDegrees.y() < 0) {
        ztrans -= 0.25f;
    }
    this->update();
    event->accept();
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{

    switch (event->key()) {
    case Qt::Key_W:
        cameraPos += cameraSpeed * cameraFront;
        break;
    case Qt::Key_S:
        cameraPos -= cameraSpeed * cameraFront;
        break;
    case Qt::Key_A:
        cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
        break;
    case Qt::Key_D:
        cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
        break;
    case Qt::Key_Q:
        cameraPos += cameraSpeed * cameraUp;
        break;
    case Qt::Key_E:
        cameraPos -= cameraSpeed * cameraUp;
        break;
    default:
        break;
    }
    this->update();   
    event->accept();
}
