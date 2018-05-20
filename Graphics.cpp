#include "Graphics.h"
using namespace std;

Vec3::Vec3()
{

}

Vec3::Vec3(float a, float b, float c)
{
    x=a;
    y=b;
    z=c;
}

float Vec3::dst(Vec3 b, Vec3 e)
{
    float dx = e.x-b.x;
    float dy = e.y-b.y;
    float dz = e.z-b.z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

float Vec3::length(Vec3 a)
{
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 Vec3::lerp(Vec3 b, Vec3 e, float s)
{
    return b+(e-b)*s;
}

Vec3 Vec3::cross(Vec3 u, Vec3 v)
{
    return Vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}

float Vec3::angleDiff(float b, float e)
{
    while (b<0) b+=360;
    while (e<0) e+=360;
    while (b>=360) b-=360;
    while (e>=360) e-=360;

    float d = e-b;
    if (d>180) d = 180 - d;
    if (d<-180) d = -180 - d;

    return d;
}

float Vec3::angleXZ()
{
    float t = atan2(z, x) * 180 / M_PI;
    if (t >= 360) t -= 360;
    return -t;
}

void Vec3::normalize()
{
    float dst = sqrt(x*x+y*y+z*z);
    x /= dst;
    y /= dst;
    z /= dst;
}

Vec3& Vec3::operator += (const Vec3& right)
{
    x += right.x;
    y += right.y;
    z += right.z;

    return *this;
}

Vec3& Vec3::operator -= (const Vec3& right)
{
    x -= right.x;
    y -= right.y;
    z -= right.z;

    return *this;
}

Vec3& Vec3::operator *= (const float right)
{
    x *= right;
    y *= right;
    z *= right;

    return *this;
}

Vec3& Vec3::operator /= (const float right)
{
    x /= right;
    y /= right;
    z /= right;

    return *this;
}

Vec3 operator + (Vec3 left, const Vec3& right)
{
    left += right;
    return left;
}

Vec3 operator - (Vec3 left, const Vec3& right)
{
    left -= right;
    return left;
}

Vec3 operator * (Vec3 left, const float right)
{
    left *= right;
    return left;
}

Vec3 operator / (Vec3 left, const float right)
{
    left /= right;
    return left;
}

ostream& operator << (ostream& out, const Vec3& right)
{
    out << "(" << right.x << ", " << right.y << ", " << right.z << ")";
    return out;
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

void Graphics::drawVertex(Vec3 a)
{
    glVertex3f(a.x,a.y,a.z);
}

void Graphics::drawQuad(Vec3 a1, Vec3 a2, Vec3 a3, Vec3 a4)
{
    drawVertex(a1);
    drawVertex(a2);
    drawVertex(a3);
    drawVertex(a4);
}

void Graphics::drawRoof()
{
    Vec3 a1(0,0,-0.05);
    Vec3 a2(0.025,0.05,-0.0375);
    Vec3 a3(0.075,0.05,-0.0375);
    Vec3 a4(0.1125,0,-0.05);
    Vec3 a5(0,0,0.05);
    Vec3 a6(0.025,0.05,0.0375);
    Vec3 a7(0.075,0.05,0.0375);
    Vec3 a8(0.1125,0,0.05);

    glPushMatrix();
    glTranslatef(-0.075, 0.025, 0);
    glBegin(GL_QUADS);

    drawQuad(a2,a6,a7,a3);

    glColor3f(0,1,1);

    drawQuad(a1,a2,a3,a4);
    drawQuad(a1,a5,a6,a2);
    drawQuad(a5,a8,a7,a6);
    drawQuad(a8,a4,a3,a7);


    glEnd();
    glPopMatrix();
}

float Graphics::lerp(float a, float b, float s)
{
    float d = b - a;
    return a + d * s;
}

float Graphics::lerpAngle(float a, float b, float s)
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

int Graphics::rotateDirection(float a, float b)
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
