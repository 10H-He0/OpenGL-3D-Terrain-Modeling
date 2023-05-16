
#ifndef TERRIAN_H
#define TERRIAN_H

#include "OpenGLWindow.h"
#include "Utilize.h"

class Terrian {
public :
    Terrian(float minheight, float maxheight);
    ~Terrian();
    GLint terrian_index[img_height*img_width*3];
    GLint terrian_pos[img_height*img_width][3];

    std::vector<MeshLib::Vec3> vertexs;
    std::vector<GLuint> triangles;

    GLfloat afterloop_pos[850][3];

    void loop();
    void loadterrian();

    static QString FilePath;

private :
    float minHeight;
    float maxHeight;
    int index;
};
#endif // TERRIAN_H
