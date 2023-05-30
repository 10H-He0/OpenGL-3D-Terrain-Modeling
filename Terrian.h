
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
    std::vector<GLint> terrian_index;
    std::vector<std::vector<GLint>> terrian_pos;

    std::vector<MeshLib::Vec3> vertexs;
    std::vector<GLuint> triangles;
    std::vector<GLuint> simplifyTriangles;
    TriMesh* trimesh;

    GLfloat afterloop_pos[100000][3];

    void loop();
    void loadterrian();
    void simplify();
    int img_width = 0;
    int img_height = 0;

private :
    float minHeight;
    float maxHeight;
    int index;
};

#endif // TERRIAN_H
