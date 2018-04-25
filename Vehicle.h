#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"
#include "Street.h"


class Road;
class Cross;
//class GameObject;

class Vehicle : public GameObject
{
public:
    Vehicle(Road *spawnRoad);
protected:
    float velocity;
    float stopTime;
    float xPos;
    //Vec3 direction;
    void update(float delta);

    Road *curRoad;
    Cross *curCross;
    Vehicle *frontVeh;
};

class Car : public Vehicle
{
public:
    Car(Road *spawnRoad);

private:

    void update(float delta);
    void draw();
};

#endif // VEHICLE_H
