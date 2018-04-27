#include "Vehicle.h"
#include "Street.h"

class Road;

int Vehicle::numVeh = 0;

Vehicle::Vehicle(Road *spawnRoad)
{
    debugstop = false;

    numVeh++;

    maxV = randFloat(1, 1.5);
    minV = randFloat(0.02, 0.08);
    velocity = 0;
    stopTime = randFloat(0.5, 0.8);
    acceleration = randFloat(0.1,0.2);
    vehicleLength = 0.2;

    xPos = 0;

    isBraking = false;

    gameEngine = spawnRoad->gameEngine;

    curRoad = spawnRoad;
    curCross = NULL;

    isChanging = false;

    if (curRoad->vehiclesBeg.size() > 0)
    {
        frontVeh = curRoad->vehiclesBeg.back();
        frontVeh->backVeh = this;
    }
}
#include<iostream>
using namespace std;
bool pierwszy = true;
Vehicle *pie;
int kla = 0;

void Vehicle::update(float delta)
{
    if (debugstop) return;

    if (!isChanging)
    {

        xPos += velocity * delta;

        float posDiff = getDst() - vehicleLength;
        float newDst = velocity * stopTime - acceleration * stopTime * stopTime / 2.0;

        if (newDst > posDiff)
        {
            velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
            velocity /= stopTime;
            isBraking = true;
        }
        else if (posDiff > newDst)
        {
            velocity = posDiff - acceleration * stopTime * stopTime / 2.0;
            velocity /= stopTime;
            //velocity += acceleration * delta;
            isBraking = false;
        }

        if (velocity < minV)
        {
            velocity = 0;
            isBraking = true;
        }

        if (velocity > maxV)
        {
            velocity = maxV;
        }

        //cout << "         "<<velocity<<endl;

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



        //cout<<id<<endl;
        if (curRoad->length - xPos < 2 && curCross == NULL)
        {
            //Cross *newCross;
            allowedToCross = false;

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
                    desiredTurn = randInt(0, curCross->streets.size()-1);
                    if (desiredTurn == i) desiredTurn = (desiredTurn+1) % curCross->streets.size();
                    nextRoad = curCross->streets[desiredTurn].street;

                    curCross->streets[i].vehicles.push_back(this);

                    break;
                }
            }
        }
    }

    //changing street at cross

    if (curCross != NULL && nextRoad != NULL && allowedToCross && nextRoad->freeSpace(direction) > 0)
    {
        if(backVeh != NULL)
        {
            backVeh->isFirstVeh = true;
            backVeh->frontVeh = NULL;
        }
        isChanging = true;
    }

    if (isChanging && curRoad != NULL)
    {

        xPos += velocity * delta;
         float s = xPos / curRoad->length;
        //cout<<xPos<<"   "<<getDst()<<endl;
        pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);

        if (s>1.5)//DEBUG
        {
            if (allowedToCross && curRoad != NULL && curCross != NULL)
            {
                debugstop = true;
                allowedToCross = false;
                cout<<"unregister&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

                if (direction)
                {
                    curRoad->vehiclesBeg.pop();
                }
                else
                {
                    curRoad->vehiclesEnd.pop();
                }
                curCross->allowedVeh--;
                curCross = NULL;
                curRoad = NULL;
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

    if (isBraking)
    {
        setColor(1,0,0);
    }
    if (curCross != NULL && allowedToCross)
    {
        setColor(0,0,1);
    }

    drawCube(0.2,4,0.2);

}
