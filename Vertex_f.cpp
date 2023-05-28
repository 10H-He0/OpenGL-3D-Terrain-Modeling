#include "Vertex_f.h"


Vertex_f::Vertex_f(void)
{
    id = -99;
    connectVertexes.clear();
}

Vertex_f::~Vertex_f(void)
{

}

void Vertex_f::addConnectVertex(int num) {
    connectVertexes.insert(num);
}

void Vertex_f::delConnectVertex(int num) {
    connectVertexes.erase(num);
}

bool Vertex_f::isExistConnectVertex(int num) {
    return (connectVertexes.count(num) > 0);
}

