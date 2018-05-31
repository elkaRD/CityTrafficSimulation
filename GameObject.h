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
    void setPos(Vec3 p);
    void setRot(Vec3 r);
    Vec3 getPos();
    Vec3 getRot();
    std::string id;

    GameObject();
    virtual ~GameObject(){};

    void updateObject(float delta);
    void drawObject();

protected:
    Vec3 pos;        //debug
    Vec3 rot;

    virtual void update(float delta);
    float randFloat(float minV, float maxV);
    int randInt(int minV, int maxV);
};

#endif // GAMEOBJECT_H
