
#ifndef QUADTREE_H
#define QUADTREE_H

#endif // QUADTREE_H

#ifndef QuadTree_h
#define QuadTree_h

#include "QuadTreeNode.h"
#include <vector>
#include <QImage>

class QuadTree {
public:
    QuadTree();
    QuadTreeNode *root;
    void initialize(QImage img);
private:
    void createLevel(std::vector< std::vector<QuadTreeNode *> > &lastLevel, int size);
};

#endif

