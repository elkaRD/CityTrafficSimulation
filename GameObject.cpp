#include "GameObject.h"

GameObject::GameObject()
{
    pos = Vec3(0,0,0);
    rot = Vec3(0,0,0);
}

/*GameObject::GameObject(Simulator *engine)
{
    pos = Vec3(0,0,0);
    rot = Vec3(0,0,0);
    gameEngine = engine;
}*/

float GameObject::randFloat(float minV, float maxV)
{
    float d = maxV - minV;
    return minV + d*(rand()%1000)/1000.0;
}

int GameObject::randInt(int minV, int maxV)
{
    int d = maxV - minV + 1;
    return minV + rand()%d;
}

void GameObject::setPos(Vec3 p)
{
    pos = p;
}

void GameObject::setRot(Vec3 r)
{
    rot = r;
}

Vec3 GameObject::getPos()
{
    return pos;
}

Vec3 GameObject::getRot()
{
    return rot;
}

void GameObject::drawObject()
{
    pushMatrix();

    translate(pos);
    rotateY(rot.y);
    rotateX(rot.x);
    rotateZ(rot.z);

    draw();
    popMatrix();
}

void GameObject::updateObject(float delta)
{
    update(delta);
}

void GameObject::update(float delta)
{

}
