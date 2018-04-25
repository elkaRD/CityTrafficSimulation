#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <cmath>
#include <iostream>

class Vec3
{
public:

    Vec3();
    Vec3(float a, float b, float c);

    float x,y,z;
    static float dst(Vec3 b, Vec3 e);
    static Vec3 lerp(Vec3 b, Vec3 e, float s);
    void normalize();
    Vec3& operator += (const Vec3& right);
    Vec3& operator -= (const Vec3& right);
    Vec3& operator *= (const float right);
    Vec3& operator /= (const float right);

    //friend std::istream& operator >> (std::istream& in, Vec3& right);
};

Vec3 operator + (Vec3 left, const Vec3& right);
Vec3 operator - (Vec3 left, const Vec3& right);
Vec3 operator * (Vec3 left, const float right);
Vec3 operator / (Vec3 left, const float right);

//std::istream& operator >> (std::istream& in, Vec3& right);

class Graphics
{
protected: public:
    virtual void draw();
    void drawCube(float a);
    void drawCube(float x, float y, float z);
    void drawLine(Vec3 begP, Vec3 endP);
    void setColor(float r, float g, float b);
};

#endif // GRAPHICS_H
