#include "Street.h"
#include<iostream>
using namespace std;

Cross::Cross(Vec3 position)
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

Street::Street(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    Cross::OneStreet temp;
    temp.street = this;

    crossBeg->streets.push_back(temp);
    crossEnd->streets.push_back(temp);
}

void Street::draw()
{
    drawLine(crossBeg->getPos(), crossEnd->getPos());
}

Garage::Garage(Vec3 p, Cross *c)
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
    temp = new Car();
    vehicles.push(temp);

    cout<<"spotted car by "<<id<<endl;
}
