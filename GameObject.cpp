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

}

GameObject::GameObject(Simulator *engine)
{
    gameEngine = engine;
}

float GameObject::randFloat(float minV, float maxV)
{
    float d = maxV - minV;
    return minV + d*(rand()%1000)/1000.0;
}

int GameObject::randInt(int minV, int maxV)
{
    int d = maxV - minV;

    if (d == 0) return minV;

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
