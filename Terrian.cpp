#include "Terrian.h"
#include "TriMesh.h"

using namespace MeshLib;

Terrian::Terrian(float minheight, float maxheight)
{
    minHeight = minheight;
    maxHeight = maxheight;
}

Terrian::~Terrian()
{

}

void Terrian::loadterrian()
{
    QImage heightmap;
    heightmap.load("D:/code/OpenGL-3D-Terrain-Modeling/heightmap.png");

    for(int i=0;i<img_width*(img_height - 1);i++)
    {
        terrian_index[index++]=i;
        terrian_index[index++]=i+img_width;
    }

    for(int j=img_width-1;j>=0;j--)
    {
        for(int i=0;i<=img_width-1;i++)
        {
            terrian_pos[img_height*(img_width-1-j)+i][0]=i;
            QColor color=heightmap.pixel(i,j);
            terrian_pos[img_height*(img_width-1-j)+i][1]=color.red()/25;
            terrian_pos[img_height*(img_width-1-j)+i][2]=-(img_width-1-j);
        }
    }

    loop();
    for (int i = 0; i < vertexs.size(); i++)
    {
        afterloop_pos[i][0] = vertexs[i].x;
        afterloop_pos[i][1] = vertexs[i].y;
        afterloop_pos[i][2] = vertexs[i].z;
    }
}

void Terrian::loop()
{
    std::vector<Vec3> points;
    int pos_size = sizeof(terrian_pos)/sizeof(terrian_pos[0]);

    for(int i = 0; i < pos_size; ++i)
        points.emplace_back(Vec3(terrian_pos[i][0],terrian_pos[i][1],terrian_pos[i][2]));

    std::vector<std::vector<int>> triVec;
    for(int i = 2;i < index - 1; i += 2)
    {
        std::vector<int> tri;

        if(i % (img_width * 2) == 0) continue;

        tri.emplace_back(terrian_index[i-2]);
        tri.emplace_back(terrian_index[i-1]);
        tri.emplace_back(terrian_index[i]);
        triVec.emplace_back(tri);

        tri.clear();
        tri.emplace_back(terrian_index[i]);
        tri.emplace_back(terrian_index[i-1]);
        tri.emplace_back(terrian_index[i+1]);
        triVec.emplace_back(tri);
    }

    TriMesh* mesh = new TriMesh();
    mesh->read_new(points,triVec);
    TriMesh* newMesh = new TriMesh();
    newMesh = TriMesh::create_subDivisionMesh(mesh);

    vertexs.clear();
    triangles.clear();
    for(int i = 0; i < newMesh->Vertexs().size(); ++i)
        vertexs.emplace_back(newMesh->Vertexs()[i]->vcoord);

    auto faces = newMesh->Faces();
    for(auto f : faces)
    {
        HalfEdge* he = f->halfEdge;

        do{
            triangles.emplace_back(he->vert->id);
            he = he->next;
        } while(he != f->halfEdge);

    }
}
