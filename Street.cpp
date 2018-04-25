#include "Street.h"
#include<iostream>
using namespace std;

Cross::Cross(Simulator *engine, Vec3 position)
{
    pos = position;
}

void Cross::update(float delta)
{

}

void Cross::draw()
{
    setColor(1,0,0);
    drawCube(2);
}

Street::Street(Simulator *engine, Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    Cross::OneStreet temp;
    temp.street = this;

    crossBeg->streets.push_back(temp);
    crossEnd->streets.push_back(temp);

    length = Vec3::dst(crossBeg->getPos(), crossEnd->getPos());
    direction = crossEnd->getPos() - crossBeg->getPos();
    direction.normalize();
}

void Street::draw()
{
    drawLine(crossBeg->getPos(), crossEnd->getPos());
}

Garage::Garage(Simulator *engine, Vec3 p, Cross *c)
{
    pos = p;
    cross = c;

    Cross::OneStreet temp;
    temp.enabled = false;

    cross->streets.push_back(temp);

    curTime = 0;
    frec = 3;
}

void Garage::draw()
{
    setColor(0,0,1);
    drawCube(1);
}

void Garage::update(float delta)
{
    //cout<<curTime<<" "<<delta<<endl;
    curTime += delta;
    if (curTime > frec)
    {
        curTime = 0;
        spotCar();
    }
}

void Garage::spotCar()
{
    Vehicle *temp;
    temp = new Car(this);
    vehiclesBeg.push(temp);

//    gameEngine->registerObject(temp);

    cout<<"spotted car by "<<id<<endl;
}
