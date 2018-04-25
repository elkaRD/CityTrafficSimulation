#include "Vehicle.h"
#include "Street.h"

class Road;

Vehicle::Vehicle(Road *spawnRoad)
{
    velocity = randFloat(2, 5);
    stopTime = randFloat(0.5 , 1.5);

    gameEngine = spawnRoad->gameEngine;

    curRoad = spawnRoad;

    if (curRoad->vehiclesBeg.size() > 0)
    {
        frontVeh = curRoad->vehiclesBeg.back();
    }
}
#include<iostream>
using namespace std;
void Vehicle::update(float delta)
{
    cout<<xPos<<endl;
    xPos += velocity * delta;
    float s = xPos / curRoad->length;
    pos = Vec3::lerp(curRoad->begPos, curRoad->endPos, s);
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
