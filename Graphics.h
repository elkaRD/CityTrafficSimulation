///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Graphics.h


#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/gl.h>
#include <cmath>
#include "Vec3.h"

class Graphics
{
protected:
    virtual void draw();
    void drawCube(float a) const;
    void drawCube(float x, float y, float z) const;
    void drawLine(const Vec3 begP, const Vec3 endP) const;
    void drawTile(float a) const;
    void setColor(const float r, const float g, const float b);
    void setColor(const Vec3 c);

    void drawVertex(const Vec3 a) const;
    void setNormal(const Vec3 a);
    void setNormal(const float x, const float y, const float z);
    void drawQuad(const Vec3 a1, const Vec3 a2, const Vec3 a3, const Vec3 a4) const;

    float lerp(float a, float b, float s) const;
    float lerpAngle(float a, float b, float s) const;
    int rotateDirection(float a, float b) const;

    static const unsigned int QUADS;
    static const unsigned int TRIANGLES;
    static const unsigned int LINES;
    static const unsigned int POLYGON;

    void beginDraw(const int mode);
    void endDraw();
    void drawTriangle(const Vec3 a1, const Vec3 a2, const Vec3 a3) const;
    void pushMatrix();
    void popMatrix();
    void rotateX(const float x);
    void rotateY(const float y);
    void rotateZ(const float z);
    void translate(const float x, const float y, const float z);
    void translate(const Vec3 t);
    void scale(const float x, const float y, const float z);
    void scale(const Vec3 s);
};

#endif // GRAPHICS_H
