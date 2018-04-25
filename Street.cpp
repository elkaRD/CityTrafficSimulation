#include "Street.h"
#include<iostream>
using namespace std;

Cross::Cross(Vec3 position)// : GameObject(engine)
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

    begPos = crossBeg->getPos();
    endPos = crossEnd->getPos();

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

    begPos = pos;
    endPos = cross->getPos();

    Cross::OneStreet temp;
    temp.enabled = false;

    gameEngine = engine;

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
    cout<<id<<"   "<<curTime<<" "<<delta<<endl;
    curTime += delta;
    if (curTime > frec)
    {
        cout<<"%%%%%%%"<<endl;
        curTime = 0;
        spotCar();
    }
}

void registerNewObject(Simulator *engine, GameObject *go);

void Garage::spotCar()
{
    Vehicle *temp;
    temp = new Car(this);
    temp->curRoad = this;
    vehiclesBeg.push(temp);
//    gameEngine->registerObject(temp);
    registerNewObject(gameEngine, temp);

    cout<<"spotted car by "<<id<<endl;
}
