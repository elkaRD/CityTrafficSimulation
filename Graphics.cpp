///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Graphics.cpp


#include "Graphics.h"
using namespace std;

const unsigned int Graphics::QUADS = GL_QUADS;
const unsigned int Graphics::TRIANGLES = GL_TRIANGLES;
const unsigned int Graphics::POLYGON = GL_POLYGON;
const unsigned int Graphics::LINES = GL_LINES;

void Graphics::draw()
{

}

void Graphics::drawCube(float a) const
{
    drawCube(a,a,a);
}

void Graphics::drawCube(float x, float y, float z) const
{
    glPushMatrix();

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

void Graphics::drawLine(const Vec3 b, const Vec3 e) const
{
    glBegin(GL_LINES);
    glVertex3f(b.x,b.y,b.z);
    glVertex3f(e.x,e.y,e.z);
    glEnd();
}

void Graphics::drawTile(float a) const
{
    a /= 2;
    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glVertex3f(-a,0,-a);
    glVertex3f(a,0,-a);
    glVertex3f(a,0,a);
    glVertex3f(-a,0,a);
    glEnd();
}

void Graphics::setColor(const float r, const float g, const float b)
{
    glColor3f(r,g,b);
}

void Graphics::setColor(const Vec3 c)
{
    glColor3f(c.x, c.y, c.z);
}

void Graphics::drawVertex(const Vec3 a) const
{
    glVertex3f(a.x,a.y,a.z);
}

void Graphics::setNormal(const float x, const float y, const float z)
{
    glNormal3f(x, y, z);
}

void Graphics::setNormal(const Vec3 a)
{
    glNormal3f(a.x, a.y, a.z);
}

void Graphics::drawQuad(const Vec3 a1, const Vec3 a2, const Vec3 a3, const Vec3 a4) const
{
    drawVertex(a1);
    drawVertex(a2);
    drawVertex(a3);
    drawVertex(a4);
}

float Graphics::lerp(const float a, const float b, const float s) const
{
    float d = b - a;
    return a + d * s;
}

float Graphics::lerpAngle(float a, float b, float s) const
{
    while (a<0) a+=360;
    while (b<0) b+=360;
    while (a>=360) a-=360;
    while (b>=360) b-=360;

    float diff = abs(b - a);
    float temp = 360 - diff;

    if (diff < temp)
    {
        if (a<b)
            return a + diff * s;
        return a - diff * s;
    }

    if (a<b)
        return a - temp * s;
    return a + temp * s;
}

int Graphics::rotateDirection(float a, float b) const
{
    while (a<0) a+=360;
    while (b<0) b+=360;
    while (a>=360) a-=360;
    while (b>=360) b-=360;

    float diff = abs(b - a);
    float temp = 360 - diff;

    if (diff < 45 || temp < 45) return 0;

    if (diff < temp)
    {
        if (a<b)
            return -1;
        return 1;
    }

    if (a<b)
        return 1;
    return -1;
}

void Graphics::beginDraw(const int mode)
{
    glBegin(mode);
}

void Graphics::endDraw()
{
    glEnd();
}

void Graphics::drawTriangle(const Vec3 a1, const Vec3 a2, const Vec3 a3) const
{
    drawVertex(a1);
    drawVertex(a2);
    drawVertex(a3);
}

void Graphics::pushMatrix()
{
    glPushMatrix();
}

void Graphics::popMatrix()
{
    glPopMatrix();
}

void Graphics::rotateX(const float x)
{
    glRotatef(x,1,0,0);
}

void Graphics::rotateY(const float y)
{
    glRotatef(y,0,1,0);
}

void Graphics::rotateZ(const float z)
{
    glRotatef(z,0,0,1);
}

void Graphics::translate(const float x, const float y, const float z)
{
    glTranslatef(x,y,z);
}

void Graphics::translate(const Vec3 t)
{
    glTranslatef(t.x,t.y,t.z);
}

void Graphics::scale(const float x, const float y, const float z)
{
    glScalef(x,y,z);
}

void Graphics::scale(const Vec3 s)
{
    glScalef(s.x,s.y,s.z);
}
