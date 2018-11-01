///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Vec3.h


#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vec3
{
public:

    Vec3();
    Vec3(const float a, const float b, const float c);

    float x,y,z;
    static float dst(const Vec3 b, const Vec3 e);
    static float length(const Vec3 a);
    static Vec3 lerp(Vec3 b, Vec3 e, float s);
    static Vec3 cross(const Vec3 u, const Vec3 v);
    static float angleDiff(float b, float e);

    float angleXZ() const;

    void normalize();

    Vec3& operator += (const Vec3& right);
    Vec3& operator -= (const Vec3& right);
    Vec3& operator *= (const float right);
    Vec3& operator /= (const float right);

    Vec3 operator - ();
};

Vec3 operator + (Vec3 left, const Vec3& right);
Vec3 operator - (Vec3 left, const Vec3& right);
Vec3 operator * (Vec3 left, const float right);
Vec3 operator / (Vec3 left, const float right);

std::ostream& operator << (std::ostream& out, const Vec3& right);

#endif // VEC3_h
