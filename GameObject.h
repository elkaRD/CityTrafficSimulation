///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: GameObject.h


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cmath>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <queue>
#include "Graphics.h"

class Simulator;

class GameObject : public Graphics
{
public:
    void setPos(const Vec3 p);
    void setRot(const Vec3 r);
    Vec3 getPos() const;
    Vec3 getRot() const;
    std::string id;

    GameObject();
    virtual ~GameObject(){};

    void updateObject(const float delta);
    void drawObject();

protected:
    Vec3 pos;        //debug
    Vec3 rot;

    virtual void update(const float delta);
    static float randFloat(const float minV, const float maxV);
    static int randInt(const int minV, const int maxV);
};

#endif // GAMEOBJECT_H
