#include"Utilize.h"
#include "Vertex_f.h"
#include "Config.h"
#include "VertexSet.h"
#include"CollapseCost.h"
CollapseCost::~CollapseCost() {

}
float CollapseCost::edgelength(Vertex_f* dot_1, Vertex_f* dot_2) {
    float res;
    MeshLib::Vec3 pos1 = dot_1->pos;
    MeshLib::Vec3 pos2 = dot_2->pos;
    res = pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2) + pow(pos1.z - pos2.z, 2);
    return res;
}
float CollapseCost::cost(VertexSet*p,Vertex_f dot_1, Vertex_f dot_2) {
    float res=0;
    for (int i = 0; i < dot_1.neighborfaces.size(); i++) {
        res += distance(p, dot_2.pos, dot_1.neighborfaces[i]);
    }

    return res;
}
void CollapseCost::initcost(VertexSet* p) {
    initinfo();
    for (int i = 0; i < p->cntVertex; i++) {
        for (set<int>::iterator it = p->group[i].connectVertexes.begin(); it != p->group[i].connectVertexes.end(); it++) {
            CostInfo temp = { {i,*it},cost(p,p->group[i],p->group[*it]) };
            costinfo.push_back(temp);
        }
    }

}
void CollapseCost::updatecost(VertexSet* p, int u, int v) {
    for (set<int>::iterator it = p->group[v].connectVertexes.begin(); it != p->group[v].connectVertexes.end(); it++) {
        for (int i = 0; i < costinfo.size(); i++) {
            if (costinfo[i].dot_id[0] == u && costinfo[i].dot_id[1] == *it) {
                costinfo.erase(costinfo.begin()+i);
                i--;
            }
            else if (costinfo[i].dot_id[0] == *it && costinfo[i].dot_id[1] == u) {
                costinfo.erase(costinfo.begin() + i);
                i--;
            }
            else if (costinfo[i].dot_id[0] == u && costinfo[i].dot_id[1] == v) {
                costinfo.erase(costinfo.begin() + i);
                i--;
            }
            else if (costinfo[i].dot_id[0] == v && costinfo[i].dot_id[1] == u) {
                costinfo.erase(costinfo.begin() + i);
                i--;
            }
        }
    }
    for (set<int>::iterator it = p->group[v].connectVertexes.begin(); it != p->group[v].connectVertexes.end(); it++) {
        CostInfo temp_1 = { {*it,v},cost(p,p->group[*it],p->group[v]) };
        CostInfo temp_2 = { {v,*it},cost(p,p->group[v],p->group[*it]) };
        bool isaddcost=true;
        for (int i = 0; i < costinfo.size(); i++) {
            if (temp_1.dot_id[0] == costinfo[i].dot_id[0] && temp_1.dot_id[1] == costinfo[i].dot_id[1]) {
                isaddcost = false;
                break;
            }
        }
        if (isaddcost) {
            costinfo.push_back(temp_1);
            costinfo.push_back(temp_2);
        }
    }
    for (int i = 0; i < costinfo.size(); i++) {
        if (costinfo[i].dot_id[0] == costinfo[i].dot_id[1]) {
            costinfo.erase(costinfo.begin() + i);
            i--;
        }
    }

}
CostInfo CollapseCost::findleastcost(VertexSet* p) {
    CostInfo res;
    res.dot_id[0] = 0; res.dot_id[1] = 1;
    res.cost = Config::INF;
    for (int i = 0; i < costinfo.size(); i++) {
        int id = costinfo[i].dot_id[0];
        if (p->group[id].isBoundary) {
            continue;
        }
        else if (costinfo[i].cost < res.cost) {
            res = costinfo[i];
        }
    }
    return res;
}
void CollapseCost::initinfo() {
    costinfo.clear();
}
float CollapseCost::distance(VertexSet *p,MeshLib::Vec3 pos, _face aimface) {
    int p1, p2, p3;
    p1 = aimface.x;
    p2 = aimface.y;
    p3 = aimface.z;
    float a = (p->group[p2].pos.y - p->group[p1].pos.y) * (p->group[p3].pos.z - p->group[p1].pos.z)- (p->group[p2].pos.z - p->group[p1].pos.z) * (p->group[p3].pos.y - p->group[p1].pos.y);
    float b = (p->group[p2].pos.z - p->group[p1].pos.z) * (p->group[p3].pos.x - p->group[p1].pos.x) - (p->group[p2].pos.x - p->group[p1].pos.x) * (p->group[p3].pos.z - p->group[p1].pos.z);
    float c = (p->group[p2].pos.x - p->group[p1].pos.x) * (p->group[p3].pos.y - p->group[p1].pos.y) - (p->group[p2].pos.y - p->group[p1].pos.y) * (p->group[p3].pos.x - p->group[p1].pos.x);
    float d = -(a * p->group[p1].pos.x + b * p->group[p1].pos.y + c * p->group[p1].pos.z);

    float dd = abs(a * pos.x + b * pos.y + c * pos.z + d) / sqrt(a * a + b * b + c * c);
    return dd;
}
