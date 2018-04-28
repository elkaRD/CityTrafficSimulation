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
    remainDst = randFloat(0.1, 0.2);

    xPos = 0;

    isBraking = false;

    gameEngine = spawnRoad->gameEngine;

    curRoad = spawnRoad;
    curCross = NULL;

    isChanging = false;
    didReachCross = false;
    isLeavingRoad = false;

    if (curRoad->vehiclesBeg.size() > 0 && !curRoad->vehiclesBeg.back()->allowedToCross)
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

    if (!isChanging && !didReachCross)
    {

        xPos += velocity * delta;

        float posDiff = getDst() - vehicleLength / 2.0 - remainDst;
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

        if(xPos > curRoad->length)
        {
            xPos = curRoad->length;
            //didReachCross = true;
        }

        float s = xPos / curRoad->length;

        if (s>1)
        {
            s=1;
        }

        //if (frontVeh!=NULL) cout<<id<<"   "<<xPos<<"   "<<frontVeh->xPos<< " "<<curRoad->length<<"  "<<getDst()<<endl;
        pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);

        if (direction)
        {
            pos += curRoad->normal * 0.1;
        }
        else
        {
            pos -= curRoad->normal * 0.1;
        }


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

    if (!isLeavingRoad && curCross != NULL && nextRoad != NULL && allowedToCross && nextRoad->freeSpace(direction) > vehicleLength + remainDst)
    {
        isLeavingRoad = true;

        if(backVeh != NULL)
        {
            backVeh->isFirstVeh = true;
            backVeh->frontVeh = NULL;
            //cout<<"prev veh: "<<backVeh->id<<"  "<<backVeh->frontVeh<<endl;
            //int i;cin>>i;
        }
        isChanging = true;
        didReachCross = false;

        cout<<id<<" warunek"<<endl;
        //xPos = 0;
    }

    /*if (isChanging && curRoad != NULL)
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
    }*/

    if (isChanging)
    {
        xPos += velocity * delta;

        float s = xPos / curRoad->length;
        cout<<xPos<<"   "<<s<<endl;
        cout<<curRoad->begPos<<curRoad->endPos<<endl;
        pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);


        if(s > 1)
        {
            xPos = 0;
            didReachCross = true;
            isChanging = false;

        }
        cout<<"changing "<<s<<endl;
        //didReachCross = true;
        //int i;cin>>i;
    }



    if (didReachCross)
    {
        xPos += velocity * delta;
        float tempLength;

            if (direction)
            {
                //cout<<"desired: "<<desiredTurn<<"   "<<curCross->streets.size()<<endl;
                tempLength = Vec3::length(curRoad->endPos - curCross->streets[desiredTurn].jointPos);
                float s = xPos / tempLength;

                if(s>1)s=1;

                cout<<"S: "<<tempLength<< " "<<curRoad->endPos << curCross->streets[desiredTurn].jointPos<<endl;

                pos = Vec3::lerp(curRoad->endPos, curCross->streets[desiredTurn].jointPos, s);
            }
            else
            {
                tempLength = Vec3::length(curRoad->begPos - curCross->streets[desiredTurn].jointPos);
                float s = xPos / tempLength;

                pos = Vec3::lerp(curRoad->begPos, curCross->streets[desiredTurn].jointPos, s);
            }

            cout<<"reachcross"<<endl;
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
