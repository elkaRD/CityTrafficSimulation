#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cmath>
#include <cstdio>
#include <string>
#include <queue>
#include "Graphics.h"



class GameObject : public Graphics
{
public:
    void setPos(Vec3 p);
    void setRot(Vec3 r);
    Vec3 getPos();
    Vec3 getRot();
    std::string id;
//private:


    void updateObject(float delta);
    void drawObject();

protected:
    Vec3 pos;
    Vec3 rot;

    virtual void update(float delta);
    //virtual void draw();
};

#endif // GAMEOBJECT_H
