#include "vertexSet.h"
#include "Vertex_f.h"
VertexSet::VertexSet()
{
    cntVertex = 0;
    isDeleted = new bool[Config::MAX_POINT_NUM];
    for (int i = 0; i < Config::MAX_POINT_NUM; i++)
        isDeleted[i] = false;
}
VertexSet::~VertexSet()
{
}

int VertexSet::addVertex(Vertex_f p) {
    cntVertex++;
    p.id = cntVertex-1;
    group[cntVertex-1] = p;
    return cntVertex-1;
}

void VertexSet::delVertex(int _id) {
    if (_id >= Config::MAX_POINT_NUM) {
        return;
    }
    isDeleted[_id] = true;//从group中删除

    //从邻接点中删除自己
    for (set<int>::iterator it = group[_id].connectVertexes.begin(); it != group[_id].connectVertexes.end(); it++) {
        group[(*it)].delConnectVertex(_id);
    }

}

int VertexSet::getCommonVertexNum(int u, int v) {
    int cnt = 0;
    for (set<int>::iterator it = group[u].connectVertexes.begin();
         it != group[u].connectVertexes.end(); it++) {//在u的邻接点中遍历
        if (group[v].isExistConnectVertex(*it)) {
            cnt++;
        }

    }
    return cnt;
}
void VertexSet::collapseEdge(int u, int v) {
    for (set<int>::iterator it = group[u].connectVertexes.begin();	it != group[u].connectVertexes.end(); it++) {//在u的邻接点中遍历
        group[v].addConnectVertex(*it);
        group[*it].addConnectVertex(v);
        group[v].neighborfaces.clear();
        for (set<int>::iterator it = group[v].connectVertexes.begin(); it != group[v].connectVertexes.end(); it++) {
            for (set<int>::iterator it_2 = group[*it].connectVertexes.begin(); it_2 != group[*it].connectVertexes.end(); it_2++)
                if (*it_2 < *it) {
                    if (group[*it_2].isExistConnectVertex(v)) {
                        _face newface = { v,*it,*it_2 };
                        group[v].neighborfaces.push_back(newface);
                    }
                    else {
                        continue;
                    }
                }
                else {
                    continue;
                }
        }
    }
    for (set<int>::iterator it = group[u].connectVertexes.begin(); it != group[u].connectVertexes.end(); ) {
        for (int i = 0; i < group[*(it)].neighborfaces.size(); i++) {
            if (group[*(it)].neighborfaces[i].y == u && group[*(it)].neighborfaces[i].z != v) {
                group[*(it)].neighborfaces[i].y = v;
            }
            else if (group[*(it)].neighborfaces[i].z == u && group[*(it)].neighborfaces[i].y != v) {
                group[*(it)].neighborfaces[i].z = v;
            }
            else if (group[*(it)].neighborfaces[i].y == u && group[*(it)].neighborfaces[i].z == v) {
                group[*(it)].neighborfaces.erase(group[*(it)].neighborfaces.begin() + i);
                i--;
            }
            else if (group[*(it)].neighborfaces[i].y == v && group[*(it)].neighborfaces[i].z == u) {
                group[*(it)].neighborfaces.erase(group[*(it)].neighborfaces.begin() + i);
                i--;
            }

        }
        group[*(it++)].delConnectVertex(u);

    }

    for (int i = 0; i < group[v].neighborfaces.size(); i++) {
        if (group[v].neighborfaces[i].y == v || group[v].neighborfaces[i].z == v) {
            group[v].neighborfaces.erase(group[v].neighborfaces.begin() + i);
            i--;
        }
    }
    group[v].delConnectVertex(v);
    group[u].connectVertexes.clear();
    group[u].neighborfaces.clear();
    isDeleted[u] = true;
}
void VertexSet::readtriangle(std::vector<GLuint> array) {
    int cout = 0;
    int id[3];
    do
    {
        for (int i = cout; i < cout + 3; i++) {
            id[i - cout] = array[i];
        }
        group[id[0]].addConnectVertex(id[1]);
        group[id[1]].addConnectVertex(id[0]);
        group[id[0]].addConnectVertex(id[2]);
        group[id[2]].addConnectVertex(id[0]);
        group[id[1]].addConnectVertex(id[2]);
        group[id[2]].addConnectVertex(id[1]);
        cout += 3;
    }
    while (cout < facecont * 3);

}
vector<int> VertexSet::outputtriangle() {
    int tempfacecont=0;
    for (int i = 0; i < cntVertex;i++) {
        if (!isDeleted[i]) {
            for (set<int>::iterator it = group[i].connectVertexes.begin(); it != group[i].connectVertexes.end(); it++) {
                if (*it < i) {
                    continue;
                }
                else {
                    for (set<int>::iterator it_2 = group[*it].connectVertexes.begin(); it_2 != group[*it].connectVertexes.end(); it_2++)
                        if (group[*it_2].isExistConnectVertex(i)) {
                            resarray.push_back(i);
                            resarray.push_back(*it);
                            resarray.push_back(*it_2);
                            tempfacecont++;
                        }
                        else {
                            continue;
                        }
                }
            }
        }
        else {
            continue;
        }
    }
    out_facecont = tempfacecont;
    return resarray;
}
void VertexSet::addneighborFace() {
    for (int i = 0; i < cntVertex; i++) {
        if (!isDeleted[i]) {
            for (set<int>::iterator it = group[i].connectVertexes.begin(); it != group[i].connectVertexes.end(); it++) {
                for (set<int>::iterator it_2 = group[*it].connectVertexes.begin(); it_2 != group[*it].connectVertexes.end(); it_2++)
                    if (*it_2 < *it) {
                        if (group[*it_2].isExistConnectVertex(i)) {
                            _face newface = { i,*it,*it_2 };
                            group[i].neighborfaces.push_back(newface);
                        }
                        else {
                            continue;
                        }
                    }
                    else {
                        continue;
                    }
            }
        }
        else {
            continue;
        }
    }
}



