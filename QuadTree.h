
#ifndef QUADTREE_H
#define QUADTREE_H

#endif // QUADTREE_H

#ifndef QuadTree_h
#define QuadTree_h
#include "OpenGLWindow.h"
#include "QuadTreeNode.h"
#include <vector>
#include <QImage>
#include <QPainter>

class QuadTree {
public:
    QuadTree();
    QuadTreeNode *root;
    void drawNode(GLint offsetX, GLint offsetY, GLint size, QuadTreeNode *node,GLint (&terrian_pos)[img_height*img_width][3]);
    void initialize(QImage img);
private:
    void createLevel(std::vector< std::vector<QuadTreeNode *> > &lastLevel, int size);
};

#endif

