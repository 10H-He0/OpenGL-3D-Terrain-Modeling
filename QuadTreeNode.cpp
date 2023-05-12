#include "QuadTreeNode.h"

#include <stdio.h>

QuadTreeNode::QuadTreeNode(){
    tlNode = 0;
    trNode = 0;
    blNode = 0;
    brNode = 0;
    parent = 0;
    min = 9000;
    max = 0;
}

// 判断该节点是否为叶子节点
bool QuadTreeNode::isLeaf(){
    return tlNode == 0 && trNode == 0 && blNode == 0 && brNode == 0;
}

// 判断该节点是否为根节点
bool QuadTreeNode::isRoot(){
    return parent == 0;
}

// 从子节点生成高度矩阵
void QuadTreeNode::generateMatrixFromChildren(){
    if(isLeaf()) return;
    int i, j, x, y;
    for(i=0; i<2; i++){
        for(j=0; j<2; j++){
            QuadTreeNode *child;
            if(i==0 && j==0){ child = tlNode; child->location = TOP_LEFT; }
            if(i==0 && j==1){ child = trNode; child->location = TOP_RIGHT; }
            if(i==1 && j==0){ child = blNode; child->location = BOTTOM_LEFT; }
            if(i==1 && j==1){ child = brNode; child->location = BOTTOM_RIGHT; }

            child->parent = this;
            // 递归生成子节点的高度矩阵
            child->generateMatrixFromChildren();
            for(x=0; x<HEIGHTMAP_SIZE; x+=2){
                for(y=0; y<HEIGHTMAP_SIZE; y+=2){
                    //printf("%d %d %d\n",x,y,child->heightmap[x][y]);
                    int temp= child->heightmap[x][y];
                    temp += child->heightmap[x+1][y];
                    temp += child->heightmap[x][y+1];
                    temp += child->heightmap[x+1][y+1];
                    if(temp%4==0||temp%4==1){
                        heightmap[(i*(HEIGHTMAP_SIZE/2))+(x/2)][(j*(HEIGHTMAP_SIZE/2))+(y/2)]=temp/4;
                    }
                    else{
                        heightmap[(i*(HEIGHTMAP_SIZE/2))+(x/2)][(j*(HEIGHTMAP_SIZE/2))+(y/2)]=temp/4+1;
                    }
                    if(child->heightmap[x][y] > max) max = child->heightmap[x][y];
                    if(child->heightmap[x][y] < min) min = child->heightmap[x][y];
                }
            }
        }
    }
    //printHeightmap();
}

void QuadTreeNode::printHeightmap(){
    int i, j;
    for(i=0; i<HEIGHTMAP_SIZE; i++){
        for(j=0; j<HEIGHTMAP_SIZE; j++){
            printf("%3d ", heightmap[i][j]);
        }
        printf("\n");
    }
}

