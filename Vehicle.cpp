#include "Vehicle.h"

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

void Vehicle::update(float delta)
{

}

Car::Car(Road *spawnRoad) : Vehicle(spawnRoad)
{
//    Vehicle(spawnRoad);
}

void Car::update(float delta)
{
    Vehicle::update(delta);
}

void Car::draw()
{
    setColor(0,1,0);
    drawCube(5);
}
