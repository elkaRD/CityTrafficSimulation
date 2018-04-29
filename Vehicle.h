#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"
//#include "Street.h"


class Road;
class Cross;
//class GameObject;

class Vehicle : public GameObject
{
public:
    Vehicle(Road *spawnRoad);
//protected:
    float maxV;
    float minV;
    float velocity;
    float cornerVelocity;
    float stopTime;
    float acceleration;
    float xPos;
    float vehicleLength;
    float remainDst;
    //float timeCorner;
    bool isBraking;
    //Vec3 direction;
    void update(float delta);

    float getDst();

    float dstToCross;

    bool direction;

    int desiredTurn;
    Road *nextRoad;
    bool allowedToCross;

    bool isChanging;
    bool didReachCross;
    bool isLeavingRoad;

    Vec3 nextRoadJoint;

    Road *curRoad;
    Cross *curCross;
    Vehicle *frontVeh;
    Vehicle *backVeh;
    bool isFirstVeh;

    int idnumber;
    bool debugstop;
    //int idnum;
    static int numVeh;
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
