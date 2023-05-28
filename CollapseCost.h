
#include"Utilize.h"
#include "Vertex_f.h"
#include "Config.h"
#include "VertexSet.h"

struct CostInfo {
    int dot_id[2];
    float cost;
};
class CollapseCost {
public:
    ~CollapseCost(void);
    vector<CostInfo> costinfo;
    void initinfo();
    float edgelength(Vertex_f*, Vertex_f* );//求边长
    float cost(VertexSet*p,Vertex_f , Vertex_f );//边（id_1，id_2)从id_1坍缩到id_2的代价评价
    void initcost(VertexSet* );//初始点对的cost
    void updatecost(VertexSet*,int,int);//对点集执行u->v的坍缩后更新每个点对的cost
    CostInfo findleastcost(VertexSet*);
    float distance(VertexSet*,MeshLib::Vec3, _face);//新的坍缩评价机制
};
