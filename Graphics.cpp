#include "Graphics.h"

Vec3::Vec3()
{

}

Vec3::Vec3(float a, float b, float c)
{
    x=a;
    y=b;
    z=c;
}

void Graphics::draw()
{

}

void Graphics::drawCube(float a)
{
    drawCube(a,a,a);
}

void Graphics::drawCube(float x, float y, float z)
{
    glPushMatrix();
    //glTranslatef(gdziex,gdziey,gdziez);
    x /= 2;
    y /= 2;
    z /= 2;

    glBegin(GL_QUADS);

    glNormal3f(0,0,-1);
    glVertex3f(-x,-y,-z);
    glVertex3f(x,-y,-z);
    glVertex3f(x,y,-z);
    glVertex3f(-x,y,-z);

    glNormal3f(1,0,0);
    glVertex3f(x,-y,-z);
    glVertex3f(x,-y,z);
    glVertex3f(x,y,z);
    glVertex3f(x,y,-z);

    glNormal3f(0,0,1);
    glVertex3f(-x,-y,z);
    glVertex3f(x,-y,z);
    glVertex3f(x,y,z);
    glVertex3f(-x,y,z);

    glNormal3f(-1,0,0);
    glVertex3f(-x,-y,-z);
    glVertex3f(-x,-y,z);
    glVertex3f(-x,y,z);
    glVertex3f(-x,y,-z);

    glNormal3f(0,1,0);
    glVertex3f(-x,y,-z);
    glVertex3f(x,y,-z);
    glVertex3f(x,y,z);
    glVertex3f(-x,y,z);

    glNormal3f(0,-1,0);
    glVertex3f(-x,-y,-z);
    glVertex3f(x,-y,-z);
    glVertex3f(x,-y,z);
    glVertex3f(-x,-y,z);
    glEnd();

    glPopMatrix();
}

void Graphics::drawLine(Vec3 b, Vec3 e)
{
    glBegin(GL_LINES);
    glVertex3f(b.x,b.y,b.z);
    glVertex3f(e.x,e.y,e.z);
    glEnd();
}

void Graphics::setColor(float r, float g, float b)
{
    glColor3f(r,g,b);
}
