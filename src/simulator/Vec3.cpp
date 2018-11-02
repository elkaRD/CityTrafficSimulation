///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Vec3.cpp


#include "Vec3.h"
using namespace std;

Vec3::Vec3()
{

}

Vec3::Vec3(const float a, const float b, const float c)
{
    x=a;
    y=b;
    z=c;
}

float Vec3::dst(const Vec3 b, const Vec3 e)
{
    float dx = e.x-b.x;
    float dy = e.y-b.y;
    float dz = e.z-b.z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

float Vec3::length(const Vec3 a)
{
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 Vec3::lerp(const Vec3 b, const Vec3 e, const float s)
{
    return b+(e-b)*s;
}

Vec3 Vec3::cross(const Vec3 u, const Vec3 v)
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

float Vec3::angleXZ() const
{
    float t = atan2(z, x) * 57.2958;
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

Vec3 Vec3::operator - ()
{
    return Vec3(-x,-y,-z);
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
