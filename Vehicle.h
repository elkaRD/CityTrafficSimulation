///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Vehicle.h


#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"
#include "Street.h"

class Driveable;
class Cross;
class Garage;

class Vehicle : public GameObject
{
public:
    Vehicle(Driveable *spawnRoad);
    virtual ~Vehicle(){};
    float getXPos() const;
    float getDstToCross() const;
    static int getNumberId();

    void initRandValues();
    struct Adjustable
    {
        float maxV;
        float minV;
        float cornerVelocity;
        float stopTime;
        float acceleration;
        float vehicleLength;
        float remainDst;
    } specs;

protected:
    float velocity;
    float xPos;
    bool isBraking;

    void update(const float delta);

    float getDst() const;
    bool isEnoughSpace() const;

    struct CrossingState
    {
        bool isChanging;
        bool didReachCross;
        bool isLeavingRoad;

        float begRot;
        float endRot;
        float crossProgress;
    } crossState;

    struct Blinker
    {
        int which;
        bool isLighting;

    private:
        float time;
        float duration;

        void init();
        void updateBlinkers(const float delta);

        friend Vehicle;
    } blinker;

private:
    void initPointers(Driveable *spawnRoad);

    void setVelocity();
    void checkVelocity(const float delta, float prevVelocity);
    void setNewPos();
    void registerToCross();

    void tryBeAllowedToEnterCross();
    void leaveRoad();
    void setCornerPosition();
    void enterNewRoad();

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
    void update(const float delta);
    void draw();
    void drawRoof();
};

class Bus : public Vehicle
{
public:
    Bus(Driveable *spawnRoad);

private:
    float busAngle;

    void update(const float delta);
    void draw();
};

#endif // VEHICLE_H
