
#ifndef TERRIAN_H
#define TERRIAN_H

#include "OpenGLWindow.h"
#include "Utilize.h"
#include "TriMesh.h"

using namespace MeshLib;

class Terrian {
public :
    Terrian(float minheight, float maxheight);
    ~Terrian();
    GLint terrian_index[img_height*img_width*3];
    GLint terrian_pos[img_height*img_width][3];

    std::vector<MeshLib::Vec3> vertexs;
    std::vector<GLuint> triangles;
    std::vector<GLuint> simplifyTriangles;
    TriMesh* trimesh;

    GLfloat afterloop_pos[850][3];

    void loop();
    void loadterrian();
    void simplify();

private :
    float minHeight;
    float maxHeight;
    int index;
};

#endif // TERRIAN_H
