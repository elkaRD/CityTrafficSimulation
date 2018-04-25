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

float GameObject::randFloat(float minV, float maxV)
{
    float d = maxV - minV;
    return minV + d*(rand()%1000)/1000.0;
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
