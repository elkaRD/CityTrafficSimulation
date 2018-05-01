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
    //return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
    left += right;
    return left;
}

Vec3 operator - (Vec3 left, const Vec3& right)
{
    //return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
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

float Graphics::lerp(float a, float b, float s)
{
    float d = b - a;
    return a + d * s;
}

float Graphics::lerpAngle(float a, float b, float s)
{
    /*while (a<0) a+=360;
    while (b<0) b+=360;

    if (a > b)
    {
        float t = a;
        a = b;
        b = t;
    }



    while (a>360) a-=360;
    while (b>360) b-=360;
    float temp = 360 - b + a;
    float diff = b - a;

    if (diff < temp) return lerp(a, b, s);

    return lerp(b-360, a, s);*/

    while (a<0) a+=360;
    while (b<0) b+=360;
    while (a>=360) a-=360;
    while (b>=360) b-=360;

    //if (a == 0 && signbit(a)) a *= -1;
    //if (b == 0 && signbit(b)) b *= -1;

    float diff = abs(b - a);
    float temp = 360 - diff;

    if (diff < temp)
    {
        //float d = abs(a-b);
        if (a<b)
            return a + diff * s;
        return a - diff * s;
    }

    if (a<b)
        return a - temp * s;
    return a + temp * s;
}
