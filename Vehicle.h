#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"
//#include "Street.h"


class Driveable;
class Cross;
//class GameObject;

class Vehicle : public GameObject
{
public:
    Vehicle(Driveable *spawnRoad);
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
    bool isEnoughSpace();

    float dstToCross;

    bool direction;

    int desiredTurn;
    Driveable *nextRoad;
    bool allowedToCross;

    bool isChanging;
    bool didReachCross;
    bool isLeavingRoad;

    Vec3 nextRoadJoint;

    float begRot;
    float endRot;
    float crossProgress;

    int blinker;
    float blinkerTime;
    float blinkerDuration;
    bool blinkerLight;

    Driveable *curRoad;
    Cross *curCross;
    Vehicle *frontVeh;
    Vehicle *backVeh;
    bool isFirstVeh;

    Vec3 vehicleColor;

    int idnumber;
    //bool debugstop;
    //int idnum;
    static int numVeh;

    void initRandValues();
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

    float busAngle;

    void update(float delta);
    void draw();
};

#endif // VEHICLE_H
