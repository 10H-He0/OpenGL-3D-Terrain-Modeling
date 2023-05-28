#ifndef VERTEX_H
#define VERTEX_H
#include "Utilize.h"
#include <vector>
#include <set>
//顶点类
struct _face {
    int x;
    int y;
    int z;
};
class Vertex_f
{
public:
    Vertex_f(void);
    ~Vertex_f(void);
    MeshLib::Vec3 pos={0,0,0};
    int id;//顶点编号
    std::set<int> connectVertexes;//邻接点
    std::vector<_face> neighborfaces;
    bool isBoundary = false;	 // 是否是边界
    void addConnectVertex(int);//增加编号为id邻接点
    void delConnectVertex(int);//删除编号为id邻接点
    bool isExistConnectVertex(int);//是否已经存在了某个邻接点
};
#endif
