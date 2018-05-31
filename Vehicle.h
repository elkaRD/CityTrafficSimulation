#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"
#include "Street.h"


class Road;
class Driveable;
class Cross;
//class GameObject;
class Garage;

class Vehicle : public GameObject
{
public:
    Vehicle(Driveable *spawnRoad);
    float getXPos();
    float getDstToCross();
    static int getNumberId();

protected:
    float maxV;
    float minV;
    float velocity;
    float cornerVelocity;
    float stopTime;
    float acceleration;
    float xPos;
    float vehicleLength;
    float remainDst;
    float maxAcceleration;
    //float timeCorner;
    bool isBraking;
    //Vec3 direction;
    void update(float delta);

    float getDst();
    bool isEnoughSpace();

    bool isChanging;
    bool didReachCross;
    bool isLeavingRoad;

    float begRot;
    float endRot;
    float crossProgress;

    int blinker;
    bool blinkerLight;



    Vec3 vehicleColor;

    //int idnumber;
    //bool debugstop;
    //int idnum;


    void initRandValues();

private:
    static int numVeh;

    float blinkerTime;
    float blinkerDuration;

    void initBlinkers();
    void initPointers(Driveable *spawnRoad);

    void setVelocity();
    void checkVelocity(float delta, float prevVelocity);
    void setNewPos();
    void registerToCross();

    void tryBeAllowedToEnterCross();
    void leaveRoad();
    void setCornerPosition();
    void enterNewRoad();

    void updateBlinkers(float delta);

    float dstToCross;

    bool direction;

    int desiredTurn;
    Driveable *nextRoad;
    bool allowedToCross;

    Vec3 nextRoadJoint;

    Driveable *curRoad;
    Cross *curCross;
    Vehicle *frontVeh;
    Vehicle *backVeh;
    bool isFirstVeh;

    friend Garage;
    friend Cross;
};

class Car : public Vehicle
{
public:
    Car(Driveable *spawnRoad);

private:

    void update(float delta);
    void draw();
};

class Bus : public Vehicle
{
public:

    Bus(Driveable *spawnRoad);

private:

    float busAngle;

    void update(float delta);
    void draw();
};

#endif // VEHICLE_H
