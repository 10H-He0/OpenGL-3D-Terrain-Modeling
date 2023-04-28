#include "TriMesh.h"
using namespace MeshLib;


// 读入点集和三角形集
void TriMesh::read_new(std::vector<Vec3> points, std::vector<std::vector<int>> triVec)
{
    // 记录面的链表
    std::list<FaceIndex> f_indexList;
    // 记录点的序号
    int v_id = 0;

    for (Vec3 point : points)
        create_vertex(point, v_id++);

    for (const auto& tri : triVec)
    {
        FaceIndex f;
        f.v[0] = tri[0];
        f.v[1] = tri[1];
        f.v[2] = tri[2];

        f_indexList.emplace_back(f);
    }

    // 创建面的半边结构
    for (auto it = f_indexList.begin(); it != f_indexList.end(); ++it)
    {
        FaceIndex f = *it;
        Vertex* vertex[3];
        vertex[0] = m_vertices[f.v[0]];
        vertex[1] = m_vertices[f.v[1]];
        vertex[2] = m_vertices[f.v[2]];
        create_face(vertex);
    }

    // 设置边界
    create_boundaryFlag();
    FaceIndex f = f_indexList.front();
}

Vertex* TriMesh::create_vertex(Vec3 point, int id)
{
    Vertex* vert = new Vertex(id, point);
    m_vertices.emplace_back(vert);
    return vert;
}

HalfEdge* TriMesh::create_edge(Vertex* v1, Vertex* v2)
{
    if (v1 == nullptr || v2 == nullptr)
        return nullptr;

    // 首先查询是否存在以这v1 v2创建的半边
    EdgeKey key(v1->id, v2->id);
    // 如果存在说明是之前创建好了的对边
    if (m_hashmap_edge.find(key) != m_hashmap_edge.end())
        return m_hashmap_edge[key];

    // 不存在 则创建v1->v2的半边以及其对偶边(v2->v1 的半边),对边保存在哈希表中以便于之后填充数据
    // 创建时对边的连接关系
    HalfEdge* he = new HalfEdge();
    HalfEdge* he_op = new HalfEdge();
    he->vert = v2;
    he->opposite = he_op;
    v1->halfEdge = he;

    he_op->vert = v1;
    he_op->opposite = he;

    // 存入哈希表
    m_hashmap_edge[EdgeKey(v1->id, v2->id)] = he;
    m_hashmap_edge[EdgeKey(v2->id, v1->id)] = he_op;

    return he;
}

Face* TriMesh::create_face(Vertex* vertexs[3])
{
    Face* face = new Face();

    HalfEdge* edges[3];
    for (int i = 0; i < 3; ++i)
        edges[i] = create_edge(vertexs[i % 3], vertexs[(i + 1) % 3]);

    for (int i = 0; i < 3; ++i)
    {
        edges[i]->next = edges[(i + 1) % 3];
        edges[i]->face = face;
        m_edges.emplace_back(edges[i]);
    }

    face->halfEdge = edges[0];
    m_faces.emplace_back(face);

    return face;
}

std::vector<Face*> TriMesh::create_faces(Face* face)
{
    std::vector<Face*> faces;
    auto vertexs = getVertexsFromFace(face);
    Vertex* newVert[3];

    // 三条边添加中点
    for (int i = 0; i < 3; ++i)
    {
        Vec3 p = (vertexs[i]->vcoord + vertexs[(i + 1) % 3]->vcoord) / 2.0;
        create_vertex(p, m_vertices.size());
    }

    // 创建另外三个三角形顶点
    Vertex* triVert1[3] = { vertexs[0],newVert[0],newVert[2] };
    Vertex* triVert2[3] = { vertexs[1],newVert[1],newVert[0] };
    Vertex* triVert3[3] = { vertexs[2],newVert[2],newVert[1] };

    // 添加四个面
    faces.emplace_back(create_face(triVert1));
    faces.emplace_back(create_face(triVert2));
    faces.emplace_back(create_face(triVert3));
    faces.emplace_back(create_face(newVert));

    return faces;
}

TriMesh* TriMesh::create_subDivisionMesh(TriMesh* oldMesh)
{
    // 创建一个新的TriMesh用于存储细分之后的结果
    TriMesh* newMesh = new TriMesh();
    auto& oldVertexs = oldMesh->Vertexs();
    // 建立旧顶点到新顶点的映射（根据id，newVertexs与旧顶点一一对应）
    std::vector<Vertex* > newVertexs;
    int vid = 0;

    // 1. 保存更新旧顶点之后的位置
    // 更新旧顶点位置，保存在新的TriMesh和newVertexs数组中
    for (int i = 0; i < oldVertexs.size(); i++)
    {
        Vec3 newPosition(0, 0, 0);

        // 内部点处理
        // v = (1 − nu)v0 + u(sum of neightbor)
        if (!oldVertexs[i]->isBoundary)
        {
            auto neighborVertexs = oldMesh->getNeighborVertexs(oldVertexs[i]);
            int n = neighborVertexs.size();
            float u = (5.0 / 8.0 - pow(3.0 / 8.0 + 1.0 / 4.0 * std::cos(2 * PAI / n), 2)) / n;

            Vec3 neighborPosition_sum(0, 0, 0);
            for (int j = 0; j < n; j++)
            {
                auto neightbor = neighborVertexs[j];
                neighborPosition_sum += neightbor->vcoord;
            }

            newPosition = oldVertexs[i]->vcoord * (1 - n * u) + neighborPosition_sum * u;
        }
        // 边界点处理
        // v = 3/4 * v0 + 1/8 * (v1 + v2)
        else
        {
            auto boundaryNeighborVert = oldMesh->getBoundaryNeighborVertexs(oldVertexs[i]);
            Vec3 nvert_sum = boundaryNeighborVert[0]->vcoord + boundaryNeighborVert[1]->vcoord;
            newPosition = oldVertexs[i]->vcoord * 0.75 + nvert_sum * 0.125;
        }

        Vertex* newVertex = newMesh->create_vertex(newPosition, vid++);
        newVertexs.emplace_back(newVertex);
    }

    // 2. 用未更新旧顶点的模型计算出新增顶点的位置,通过扫描所有边，插入新的顶点
    // 建立边到新插入点的映射
    std::unordered_map<EdgeKey, Vertex*, EdgeKeyHashFuc> map_ev;
    auto oldEdges = oldMesh->HalfEges();

    for (int i = 0; i < oldEdges.size(); i++)
    {
        HalfEdge* he = oldEdges[i];
        Vertex* v1 = he->next->next->vert;
        Vertex* v2 = he->vert;
        EdgeKey key(v1->id, v2->id);

        // 半边所在的边已经创建了点
        if (map_ev.find(key) != map_ev.end())
            continue;

        // 内部边插入点
        if (!he->isBoundary)
        {
            Vertex* v3 = he->next->vert;
            Vertex* v4 = he->opposite->next->vert;
            Vec3 newPos = (v1->vcoord + v2->vcoord) * 0.375 + (v3->vcoord + v4->vcoord) * 0.125;
            Vertex* newVertex = newMesh->create_vertex(newPos, vid++);
            map_ev[EdgeKey(v1->id, v2->id)] = newVertex;
            map_ev[EdgeKey(v2->id, v1->id)] = newVertex;
        }
        // 内部边插入点
        else
        {
            Vec3 newPos = (v1->vcoord + v2->vcoord) / 2.0;
            Vertex* newVertex = newMesh->create_vertex(newPos, vid++);
            map_ev[EdgeKey(v1->id, v2->id)] = newVertex;
            map_ev[EdgeKey(v2->id, v1->id)] = newVertex;
        }
    }

    // 3. 建立新增顶点和更新之后的旧顶点之间的拓扑关系，连接成网格
    // 使用旧的Trimesh的面中的顶点信息创建面并保存，新创建的面是原始TriMesh的四倍
    auto oldFaces = oldMesh->Faces();

    for (int i = 0; i < oldFaces.size(); i++)
    {
        Face* face = oldFaces[i];
        HalfEdge* fhe[3];
        fhe[0] = face->halfEdge;
        fhe[1] = fhe[0]->next;
        fhe[2] = fhe[1]->next;
        Vertex* center[3];
        // 根据线段找到刚刚新创建的点
        for (int i = 0; i < 3; i++) {
            auto key = getHalfEdgeKey(fhe[i]);
            center[i] = map_ev[key];
        }

        // 创建以三个边的中心点为中点的三角形面片
        newMesh->create_face(center);
        Vertex* triVert[3];

        // 创建另外三个三角形面片
        for (int i = 0; i < 3; i++)
        {
            int oldVertexId = fhe[i]->next->next->vert->id;
            triVert[0] = newVertexs[oldVertexId];
            triVert[1] = map_ev[getHalfEdgeKey(fhe[i])];
            triVert[2] = map_ev[getHalfEdgeKey(fhe[(i + 2) % 3])];
            newMesh->create_face(triVert);
        }
    }

    // 创建边界标记
    newMesh->create_boundaryFlag();
    return newMesh;
}


void TriMesh::create_boundaryFlag()
{
    for (int i = 0; i < m_edges.size(); ++i)
    {
        HalfEdge* he = m_edges[i];
        HalfEdge* he_op = he->opposite;
        if (he_op->face == nullptr)
        {
            he->isBoundary = true;
            he->vert->isBoundary = true;
            he->next->next->vert->isBoundary = true;
        }
    }
}

std::vector<Vertex*> TriMesh::getVertexsFromFace(const Face* face)
{
    std::vector<Vertex*> vertexs3;
    HalfEdge* he = face->halfEdge;
    do {
        vertexs3.emplace_back(he->vert);
        he = he->next;
    } while (he != face->halfEdge);

    return vertexs3;
}

std::vector<HalfEdge*> TriMesh::getEdgesFromFace(const Face* face)
{
    std::vector<HalfEdge*> edges3;
    HalfEdge* he = face->halfEdge;
    do {
        edges3.emplace_back(he);
        //std::cout << "dd " << he->vert->id << " ";
        he = he->next;
    } while (he != face->halfEdge);

    return edges3;
}

std::vector<Vertex*> TriMesh::getNeighborVertexs(const Vertex* vertex)
{
    std::vector<Vertex*> neighbors;

    auto faces = getFacesFromVertex(vertex);
    for (int i = 0; i < faces.size(); ++i)
    {
        auto vertexs = getVertexsFromFace(faces[i]);

        for (int j = 0; j < vertexs.size(); ++j)
        {
            bool isInsert = true;
            for (int t = 0; t < neighbors.size(); t++)
                if (vertexs[j]->id == neighbors[t]->id)
                    isInsert = false;

            isInsert = isInsert && (vertexs[j]->id != vertex->id);
            if (isInsert)
                neighbors.emplace_back(vertexs[j]);
        }
    }

    return neighbors;
}

std::vector<Vertex*> TriMesh::getBoundaryNeighborVertexs(const Vertex* vertex)
{
    std::vector<Vertex*> boundaryVertex;
    auto faces = getFacesFromVertex(vertex);

    for (int i = 0; i < faces.size(); i++)
    {
        auto edges = getEdgesFromFace(faces[i]);

        for (int j = 0; j < edges.size(); j++)
        {
            if (edges[j]->isBoundary == true)
            {
                Vertex* from = getEdgeFrontVert(edges[j]);
                if (edges[j]->vert->id == vertex->id)
                    boundaryVertex.emplace_back(from);
                else if (from->id == vertex->id)
                    boundaryVertex.emplace_back(edges[j]->vert);
            }

        }

    }
    return boundaryVertex;
}

std::vector<HalfEdge*> TriMesh::getEdgesFromVertex(const Vertex* vertex)
{
    std::vector<HalfEdge*> halfEdges;

    HalfEdge* he = vertex->halfEdge;
    HalfEdge* phe = he;

    do
    {
        if (phe->isBoundary) {
            halfEdges.emplace_back(phe);
            break;
        }
        halfEdges.push_back(phe);
        phe = phe->opposite->next;
    } while (phe != he);

    if (phe->isBoundary)// 遇到边界
    {
        phe = he->next->next;
        do
        {
            if (phe->isBoundary || phe == NULL) {
                break;
            }

            halfEdges.push_back(phe->opposite);
            phe = phe->opposite->next->next;
        } while (phe->vert->id != he->vert->id);
    }
    return halfEdges;
}

std::vector<Face*> TriMesh::getFacesFromVertex(const Vertex* vertex)
{
    std::vector<Face*> faces;

    auto edges = getEdgesFromVertex(vertex);

    for (int i = 0; i < edges.size(); i++)
        faces.emplace_back(edges[i]->face);

    return faces;
}

Vertex* TriMesh::getEdgeFrontVert(HalfEdge* he)
{
    if (he == nullptr)
        return nullptr;
    return he->next->next->vert;
}

EdgeKey TriMesh::getHalfEdgeKey(HalfEdge* he)
{
    if (he == NULL)
        return EdgeKey();

    Vertex* v1 = he->next->next->vert;
    Vertex* v2 = he->vert;
    EdgeKey key(v1->id, v2->id);

    return key;
}
