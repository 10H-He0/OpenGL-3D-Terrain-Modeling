QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CollapseCost.cpp \
    Config.cpp \
    OpenGLWindow.cpp \
    QuadTree.cpp \
    QuadTreeNode.cpp \
    Terrian.cpp \
    TriMesh.cpp \
    VertexSet.cpp \
    Vertex_f.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CollapseCost.h \
    Config.h \
    HalfEdgeStruct.h \
    OpenGLWindow.h \
    QuadTree.h \
    QuadTreeNode.h \
    Terrian.h \
    TriMesh.h \
    Utilize.h \
    VertexSet.h \
    Vertex_f.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

LIBS += -lopengl32 \
        -lGLU32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
