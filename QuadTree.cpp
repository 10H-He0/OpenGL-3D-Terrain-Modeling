#include "QuadTree.h"

#include <math.h>
#include <vector>
using namespace std;

QuadTree::QuadTree(){
    root=0;
}
void QuadTree::drawNode(GLint offsetX, GLint offsetY, GLint size, QuadTreeNode *node, vector<vector<GLint>>terrian_pos, int img_width, int img_height){//GLint (&terrian_pos)[img_height*img_width][3]){
    //printf("%d %d %d\n",size,offsetX,offsetY);
    // 检查节点大小，如果节点大小小于等于地形块的大小，将节点高度值赋给对应的地形块
    if(size<=HEIGHTMAP_SIZE/2){
        //drawHeightmap(size*i+offsetX, size*j+offsetY, size, child);
//        for(int x=0; x<HEIGHTMAP_SIZE; x++){
//            for(int y=0; y<HEIGHTMAP_SIZE; y++){
//                int realx=x+offsetX,realy=y+offsetY;
//                if(realx>=img_height||realy>=img_height||realx<0||realy<0)continue;
//                terrian_pos[img_height*(img_width-1-realy)+realx][1]=node->heightmap[x][y]/25;
//            }
//        }
    }
    // 如果节点大小大于地形块的大小，递归地绘制子节点
    else{
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                QuadTreeNode *child;
                if(i==0 && j==0) child = node->tlNode;
                if(i==0 && j==1) child = node->trNode;
                if(i==1 && j==0) child = node->blNode;
                if(i==1 && j==1) child = node->brNode;
                GLfloat tile_x = size*i+offsetX;
                GLfloat tile_y = size*j+offsetY;
                drawNode(offsetX+i*size,offsetY+j*size, size/2, child,terrian_pos, img_width, img_height);
            }
        }
    }
}
void QuadTree::initialize(QImage img, int img_width, int img_height){
    //printf("%d\n",img.height());
    if(img.height()==15){
        QImage newImage(16, 16, QImage::Format_RGB32);

        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), img);

        painter.setPen(Qt::black);
        painter.drawLine(0, 15, 15, 15);
        painter.drawLine(15, 0, 15, 15);
        img=newImage;
    }
    for(int i=0; i<img_width+1; i++){
        for(int j=0; j<img_width+1; j++){
            QColor color=img.pixel(i,j);
            printf("%3d ", color.red());
        }
        printf("\n");
    }
    int height=img.height();
    int dimension = height / HEIGHTMAP_SIZE;
    printf("%d %d\n",height,dimension);

    // 创建叶子节点
    QuadTreeNode* nullNode = 0;
    vector< vector<QuadTreeNode *> > leaves(dimension, vector<QuadTreeNode *>(dimension, nullNode));

    int i, j, x, y, offsetx, offsety;
    // 按照地形块的大小划分图片，为每个地形块创建一个叶子节点
    for(i=0; i<dimension; i++){
        offsetx = HEIGHTMAP_SIZE * i;
        for(j=0; j<dimension; j++){
            offsety = HEIGHTMAP_SIZE * j;
            QuadTreeNode *node = new QuadTreeNode();

            for(x=0; x<HEIGHTMAP_SIZE; x++){
                for(y=0; y<HEIGHTMAP_SIZE; y++){

                    QColor color=img.pixel(x+offsetx,y+offsety);
                    vector<int>data;

                    data.push_back(color.red());
                    node->heightmap[x][y] = data.at(0);
                    if(data.at(0) < node->min) node->min = data.at(0);
                    if(data.at(0) > node->max) node->max = data.at(0);
                }
            }

            leaves.at(i).at(j) = node;
        }
    }


    // 递归创建节点
    createLevel(leaves, dimension);

    // 创建根节点
    root = new QuadTreeNode();
    root->tlNode = leaves.at(0).at(0);
    root->trNode = leaves.at(0).at(1);
    root->blNode = leaves.at(1).at(0);
    root->brNode = leaves.at(1).at(1);
    root->generateMatrixFromChildren();

    // 输出根节点和子节点的高度图
    printf("root:\n");
    root->printHeightmap();
    printf("tl:\n");
    root->tlNode ->printHeightmap();
    printf("tr:\n");
    root->trNode ->printHeightmap();
    printf("bl:\n");
    root->blNode ->printHeightmap();
    printf("br:\n");
    root->brNode ->printHeightmap();
}

void QuadTree::createLevel(std::vector< std::vector<QuadTreeNode*> >& lastLevel, int size){
    if(size <= 2)
        return;

    int newSize = size/2;

    int i, j;
    for(i=0; i<newSize; i++){
        for(j=0; j<newSize; j++){
            QuadTreeNode *node = new QuadTreeNode();
            node->tlNode = lastLevel[i*2][j*2];
            node->trNode = lastLevel[(i*2)][j*2+1];
            node->blNode = lastLevel[i*2+1][(j*2)];
            node->brNode = lastLevel[(i*2)+1][(j*2)+1];
            lastLevel.at(i).at(j) = node;
        }
    }
    createLevel(lastLevel, newSize);
}

