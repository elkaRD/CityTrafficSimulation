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
    drawCube(2, 0.1, 2);
}

Street::Street(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    Cross::OneStreet temp;
    temp.street = this;
    temp.enabled = true;
    temp.direction = true;

    crossBeg->streets.push_back(temp);

    temp.direction = false;
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
    crossEnd = c;
    crossBeg = NULL;

    begPos = pos;
    endPos = crossEnd->getPos();

    length = Vec3::dst(begPos, endPos);

    Cross::OneStreet temp;
    temp.enabled = false;

    gameEngine = engine;

    crossEnd->streets.push_back(temp);

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
    //cout<<id<<"   "<<curTime<<" "<<delta<<endl;
    if (vehiclesBeg.size() > 0)
    cout<< vehiclesBeg.back()->xPos <<endl;
    if (vehiclesBeg.size() == 0 || (vehiclesBeg.size() > 0 && vehiclesBeg.back()->xPos > 1))
    curTime += delta;
    if (curTime > frec)
    {
        //cout<<"%%%%%%%"<<endl;
        curTime = 0;
        spotCar();
    }
}

void registerNewObject(Simulator *engine, GameObject *go);

//int number = 0;
void Garage::spotCar()
{
    //number ++;
    //if (number > 3) return;
    Vehicle *temp;
    temp = new Car(this);
    temp->curRoad = this;
    temp->direction = true;
    vehiclesBeg.push(temp);
//    gameEngine->registerObject(temp);
    registerNewObject(gameEngine, temp);

    cout<<"spotted car by "<<id<<endl;
}
