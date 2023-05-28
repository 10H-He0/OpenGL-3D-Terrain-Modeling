#ifndef TRIMESH_H
#define TRIMESH_H
#include <assert.h>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <QDebug>

#include "HalfEdgeStruct.h"
namespace MeshLib
{
struct HalfEdge;
struct Vertex;
struct Face;

struct FaceIndex
{
    int v[3];
};

struct EdgeKey
{
    EdgeKey(int vd1 = -1, int vd2 = -1)
        :v1(vd1), v2(vd2) {}

    bool operator==(const EdgeKey& key) const
    {
        return (v1 == key.v1 && v2 == key.v2) ||
               (v1 == key.v2 && v2 == key.v1);
    }

    int v1, v2;
};

struct EdgeKeyHashFuc
{
    std::size_t operator()(const EdgeKey& key) const
    {
        return std::hash<int>()(key.v1) + std::hash<int>()(key.v2);
    }
};

class  TriMesh
{
public:
    void read_new(std::vector<Vec3> points, std::vector<std::vector<int>> triVec);
    Vertex* create_vertex(Vec3 point, int id);
    HalfEdge* create_edge(Vertex* v1, Vertex* v2);
    Face* create_face(Vertex* vertexs[3]);
    // 基于一个已存在的面根据边的中点 创建四个面
    std::vector<Face*> create_faces(Face* face);
    // 从输入的mesh 创建细分之后的三角网格
    static TriMesh* create_subDivisionMesh(TriMesh* oriMesh);
    // 找到所有边界边，并标记
    void create_boundaryFlag();
    // 获得面上的所有点，3个
    std::vector<Vertex*> getVertexsFromFace(const Face* face);
    // 获得面上的所有半边，3个
    std::vector<HalfEdge*> getEdgesFromFace(const Face* face);
    // 获得该点的所有邻居
    std::vector<Vertex*> getNeighborVertexs(const Vertex* vertex);
    // 获得该点的所有边界相邻点
    std::vector<Vertex*> getBoundaryNeighborVertexs(const Vertex* vertex);
    // 获得改点出发的所有半边
    // ?
    std::vector<HalfEdge*> getEdgesFromVertex(const Vertex* vertex);
    // 获得该点的相邻面
    std::vector<Face*> getFacesFromVertex(const Vertex* vertex);
    // 获得半边指出的点
    Vertex* getEdgeFrontVert(HalfEdge* he);

    TriMesh() {}
    const std::vector<HalfEdge*>& HalfEges() { return m_edges; }
    const std::vector<Vertex*>& Vertexs() { return m_vertices; }
    const std::vector<Face*>& Faces() { return m_faces; }
private:
    std::vector<HalfEdge*> m_edges;
    std::vector<Face*> m_faces;
    std::vector<Vertex*> m_vertices;

    std::unordered_map<EdgeKey, HalfEdge*, EdgeKeyHashFuc> m_hashmap_edge;
    static EdgeKey getHalfEdgeKey(HalfEdge* he);
};
}
#endif // TRIMESH_H
