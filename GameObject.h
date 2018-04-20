#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cmath>

struct Vec3
{
    float x,y,z;
};

class GameObject
{
public:
    void setPos(Vec3 p);
    void setRot(Vec3 r);
    Vec3 getPos();
    Vec3 getRot();
private:
    Vec3 pos;
    Vec3 rot;

    void updateObject();

protected:
    virtual void update();
    virtual void draw();
};

#endif // GAMEOBJECT_H
