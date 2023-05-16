#include "Terrian.h"
#include "TriMesh.h"
#include "QuadTree.h"
#include "QuadTreeNode.h"
using namespace MeshLib;
QuadTree *quadTree;
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
    quadTree=new QuadTree();
    QImage heightmap;
    heightmap.load("D:/code/OpenGL-3D-Terrain-Modeling/heightmap.png");
    //heightmap.load("D:/gittest/test_c/3d(1)/OpenGL-3D-Terrain-Modeling/heightmap.png");

//注释部分为未经过调试的打开文件方式
//    if(!Terrian::FilePath.isEmpty())
//    {
//        heightmap.load(Terrian::FilePath);
//    }

    quadTree->initialize(heightmap);
    //printf("%d\n",quadTree->root->heightmap[0][0]);
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
            //            QColor color=heightmap.pixel(i,j);
            //            terrian_pos[img_height*(img_width-1-j)+i][1]=color.red()/25;



            //            int height=quadTree->root->heightmap[i][j];
            //            terrian_pos[img_height*(img_width-1-j)+i][1]=height/25;

            //           printf("%d %d\n",color.red(),height);
            terrian_pos[img_height*(img_width-1-j)+i][2]=-(img_width-1-j);
        }
    }
    quadTree->drawNode(0,0,(img_width+1)/2,quadTree->root,terrian_pos);

    //    for(int j=img_width-1;j>=0;j--)
    //    {
    //        for(int i=0;i<=img_width-1;i++)
    //        {
    //           printf("%3d",terrian_pos[img_height*(img_width-1-j)+i][1]);
    //        }
    //        printf("\n");
    //    }

    //    printf("\n");
    //    for(int i=0; i<img_width; i++){
    //        for(int j=0; j<img_width; j++){

    //           printf("%3d ", terrian_pos[i][j][1]);
    //        }
    //        printf("\n");
    //    }

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
