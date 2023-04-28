
#ifndef HALFEDGESTRUCT_H
#define HALFEDGESTRUCT_H
#include "Utilize.h"

namespace MeshLib
{
struct HalfEdge;
struct Vertex;
struct Face;
struct HalfEdge
{
    HalfEdge() :vert(nullptr), next(nullptr), opposite(nullptr), face(nullptr), isBoundary(false) {}

    // 半边指向的顶点
    Vertex* vert;
    // 半边的下一条半边
    HalfEdge* next;
    // 半边的对偶边
    HalfEdge* opposite;
    // 半边所在的面
    Face* face;
    // 是否是边界
    bool isBoundary = false;
};

struct Vertex
{
    Vertex(int ID = -1, Vec3 v = Vec3(0, 0, 0)) :id(ID), vcoord(v), halfEdge(nullptr), isBoundary(false) {}

    // 索引
    int id;
    // 坐标
    Vec3 vcoord;
    // 从点发出的一个半边
    HalfEdge* halfEdge;
    // 是否是边界
    bool isBoundary = false;
};

struct Face
{
    Face(HalfEdge* he = nullptr) :halfEdge(he) {}

    // 索引
    int id;
    // 面的半边
    HalfEdge* halfEdge;
};
}
#endif // HALFEDGESTRUCT_H
