#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>

class Graphics
{
protected:
    virtual void draw();
    void drawCube(float a);
    void drawCube(float x, float y, float z);
    void setColor(float r, float g, float b);
};

#endif // GRAPHICS_H
