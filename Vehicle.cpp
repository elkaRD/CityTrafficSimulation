#include "Vehicle.h"
#include "Street.h"

class Road;

Vehicle::Vehicle(Road *spawnRoad)
{
    maxV = velocity = randFloat(2, 5);
    stopTime = randFloat(0.5 , 1.0);
    acceleration = randFloat(0.8,1.2);
    vehicleLength = 0.2;

    xPos = 0;

    gameEngine = spawnRoad->gameEngine;

    curRoad = spawnRoad;

    if (curRoad->vehiclesBeg.size() > 0)
    {
        frontVeh = curRoad->vehiclesBeg.back();
    }
}
#include<iostream>
using namespace std;
//bool pierwszy = true;
//Vehicle *pie;
//int kla = 0;
void Vehicle::update(float delta)
{
    xPos += velocity * delta;

    float posDiff = getDst() - vehicleLength;
    float newDst = velocity * stopTime - acceleration * stopTime * stopTime / 2.0;

    if (newDst > posDiff)
    {
        velocity = newDst - acceleration * stopTime * stopTime / 2.0;
        velocity /= stopTime;
    }
    else if (posDiff > newDst)
    {
        velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
        velocity /= stopTime;
    }

    if (velocity > maxV)
    {
        velocity = maxV;
    }

    /*if ((xPos > 2 && pierwszy) || pie == this)
    {xPos = 2;
    pie= this;
    pierwszy = false;
    kla++;
    if (kla > 300)
        pie = NULL;
    }*/

    float s = xPos / curRoad->length;
    //cout<<xPos<<"   "<<getDst()<<endl;
    pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);

    if (curRoad->length - xPos < 2 && curCross == NULL)
    {
        //Cross *newCross;
        if(direction)
        {
            curCross = curRoad->crossEnd;
        }
        else
        {
            curCross = curRoad->crossBeg;
        }

        if (curCross != NULL)
        for (int i=0;i<curCross->streets.size();i++)
        {
            if (curCross->streets[i].street == curRoad)
            {
                //desiredTurn = randInt(0, curCross->streets.size()-1)
                curCross->streets[i].vehicles.push(this);

                break;
            }
        }
    }
}

float Vehicle::getDst()
{
    if (frontVeh != NULL)
        return frontVeh->xPos - xPos;

    return curRoad->length - xPos;
}

Car::Car(Road *spawnRoad) : Vehicle(spawnRoad)
{
//    Vehicle(spawnRoad);
    velocity = randFloat(2,5);
}

void Car::update(float delta)
{
    Vehicle::update(delta);
}

void Car::draw()
{
    setColor(0,1,0);
    drawCube(0.2,4,0.2);
}
