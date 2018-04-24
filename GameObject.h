#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cmath>
#include <cstdio>
#include <string>
#include <queue>
#include "Graphics.h"

class Vec3
{
public:

    Vec3();
    Vec3(float a, float b, float c);

    float x,y,z;
    //friend std::istream& operator >> (std::istream& in, Vec3& right);
};

//std::istream& operator >> (std::istream& in, Vec3& right);

class GameObject : public Graphics
{
public:
    void setPos(Vec3 p);
    void setRot(Vec3 r);
    Vec3 getPos();
    Vec3 getRot();
    std::string id;
private:


    void updateObject();

protected:
    Vec3 pos;
    Vec3 rot;

    virtual void update();

};

#endif // GAMEOBJECT_H
