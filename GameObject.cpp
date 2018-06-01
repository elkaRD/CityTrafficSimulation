///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: GameObject.cpp


#include "GameObject.h"

GameObject::GameObject()
{
    pos = Vec3(0,0,0);
    rot = Vec3(0,0,0);
}

float GameObject::randFloat(const float minV, const float maxV)
{
    float d = maxV - minV;
    return minV + d*(rand()%1000)/1000.0;
}

int GameObject::randInt(const int minV, const int maxV)
{
    int d = maxV - minV + 1;
    return minV + rand()%d;
}

void GameObject::setPos(const Vec3 p)
{
    pos = p;
}

void GameObject::setRot(const Vec3 r)
{
    rot = r;
}

Vec3 GameObject::getPos() const
{
    return pos;
}

Vec3 GameObject::getRot() const
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

void GameObject::updateObject(const float delta)
{
    update(delta);
}

void GameObject::update(const float delta)
{

}
