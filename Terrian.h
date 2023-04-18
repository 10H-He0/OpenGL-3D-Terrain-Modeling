
#ifndef TERRIAN_H
#define TERRIAN_H

#include "OpenGLWindow.h"

class Terrian {
public :
    Terrian(float minheight, float maxheight);
    ~Terrian();
    GLint terrian_index[img_height*img_width*3];
    GLint terrian_pos[img_height*img_width][3];

private :
    float minHeight;
    float maxHeight;
};

#endif // TERRIAN_H
