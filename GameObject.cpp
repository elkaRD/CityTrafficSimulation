#include "GameObject.h"

/*std::istream& operator >> (std::istream& in, Vec3& right)
{
    //in >> right.x >> right.y >> right.z;
    //in >> right.x;
    int tx;
    in >> tx;
    return in;
}
*/

GameObject::GameObject()
{
    pos = Vec3(0,0,0);
    rot = Vec3(0,0,0);
}

GameObject::GameObject(Simulator *engine)
{
    pos = Vec3(0,0,0);
    rot = Vec3(0,0,0);
    gameEngine = engine;
}

float GameObject::randFloat(float minV, float maxV)
{
    float d = maxV - minV;
    return minV + d*(rand()%1000)/1000.0;
    //return (maxV+minV)/2.0; //DEBUG   TODO
}

int GameObject::randInt(int minV, int maxV)
{
    int d = maxV - minV + 1;

    //if (d == 0) return minV;

    return minV + rand()%d;
    //return (minV + maxV)/2.0; //DEBUG   TODO
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
    glPushMatrix();
    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rot.y, 0, 1, 0);
    glRotatef(rot.x, 1, 0, 0);
    glRotatef(rot.z, 0, 0, 1);

    draw();
    glPopMatrix();
}

void GameObject::updateObject(float delta)
{
    update(delta);
}

void GameObject::update(float delta)
{

}
