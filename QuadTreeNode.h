#ifndef QUADTREENODE_H
#define QUADTREENODE_H




#define HEIGHTMAP_SIZE 4

class QuadTreeNode {
public:
    QuadTreeNode();
    int heightmap[HEIGHTMAP_SIZE][HEIGHTMAP_SIZE];
    QuadTreeNode *tlNode;
    QuadTreeNode *trNode;
    QuadTreeNode *blNode;
    QuadTreeNode *brNode;
    QuadTreeNode *parent;
    int max;
    int min;

    int location;
    static const int TOP_LEFT = 0;
    static const int TOP_RIGHT = 1;
    static const int BOTTOM_LEFT = 2;
    static const int BOTTOM_RIGHT = 3;

    bool isLeaf();
    bool isRoot();
    void generateMatrixFromChildren();
    void printHeightmap();
};
#endif // QUADTREENODE_H
