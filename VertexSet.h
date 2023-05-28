#ifndef VERTEXSET_H
#define VERTEXSET_H
#include "Vertex_f.h"
#include "config.h"
#include <QOpenGLWidget>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
//所有顶点
class VertexSet
{
public:
    VertexSet();
    ~VertexSet();
    Vertex_f group[Config::MAX_POINT_NUM];
    int cntVertex=0;
    int facecont = 18;
    int out_facecont = 0;
    vector<int> resarray;
    bool* isDeleted;//判断某个顶点是否已经被删除
    int addVertex(Vertex_f);//向Set中增加一个顶点,同时根据cnt记录此节点id，返回新加入的结点id
    void delVertex(int);//在Set中删除id顶点
    void readtriangle(std::vector<GLuint> array);//读三角形数组
    vector<int> outputtriangle();
    int getCommonVertexNum(int, int);
    void collapseEdge(int, int);//使边uv坍缩为点v
    void addneighborFace();

};
#endif
