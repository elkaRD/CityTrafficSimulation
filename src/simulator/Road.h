///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Road.h


#ifndef ROAD_H
#define ROAD_H

#include "GameObject.h"
#include "Vehicle.h"
#include <sstream>

class Cross;
class Vehicle;
class Simulator;
class ObjectsLoader;

class Road : public GameObject
{
public:
    static Vec3 roadColor;
protected:
    virtual ~Road(){};
};

class Driveable : public Road
{
public:
    Vec3 getJointPoint(const bool dir) const;
    Vec3 getNormal() const;
    Vec3 getDirection() const;
    float getLength() const;

protected:
    Driveable(Cross *begCross, Cross *endCross);
    Driveable(Vec3 p, Cross *endCross);
    virtual ~Driveable(){};

    std::queue<Vehicle*> vehiclesBeg;
    std::queue<Vehicle*> vehiclesEnd;

    Vec3 begPos;
    Vec3 endPos;

    Vec3 begJoint;
    Vec3 endJoint;

    Vec3 getBegJointWidth(const bool dir) const;
    Vec3 getEndJointWidth(const bool dir) const;

    Vec3 direction;
    Vec3 normal;

    float length;

    virtual float freeSpace(const bool dir) const;

    Cross* crossBeg;
    Cross* crossEnd;

    void draw();

private:
    float reservedSpaceBeg;
    float reservedSpaceEnd;

    void commonConstructor();

    friend Vehicle;
};

class Street : public Driveable
{
public:
    Street(Cross *begCross, Cross *endCross);

private:
    void draw();
};

class Cross : public Road
{
public:
    Cross(Vec3 position);
    virtual void setDefaultPriority(Driveable *s0 = nullptr, Driveable *s1 = nullptr, Driveable *s2 = nullptr, Driveable *s3 = nullptr);

protected:
    virtual ~Cross(){};

    struct OneStreet
    {
        Driveable *street;
        std::vector<Vehicle*> vehicles;
        bool direction;
        Vec3 getJointPos();

        std::vector<std::vector<int> > yield;
    };
    std::vector<OneStreet> streets;

    virtual void updateCross(const float delta);
    virtual bool dontCheckStreet(const int which);

    virtual void tryPassVehiclesWithPriority();
    virtual void tryPassAnyVehicle();

    void draw();

private:
    bool isSet;
    int allowedVeh;

    bool checkSet();
    void update(const float delta);

    friend Driveable;
    friend Vehicle;
    friend ObjectsLoader;
};

class CrossLights : public Cross
{
public:
    CrossLights(Vec3 position);
    void setLightsDurations();

    struct LightsDuration
    {
        float durationGreen1;
        float durationGreen2;
        float durationYellow1;
        float durationYellow2;
        float durationBreak;
    } durLight;

private:
    std::vector<bool> defaultPriority;
    std::vector<bool> curPriority;

    void setDefaultPriority(Driveable *s0 = NULL, Driveable *s1 = NULL, Driveable *s2 = NULL, Driveable *s3 = NULL);
    void setDefaultLights(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3);
    void setLightsPriority();

    float curTime;

    enum State{G1, Y1, B1, G2, Y2, B2};
    State curState;
    void getNextState();

    bool dontCheckStreet(const int which);

    void update(const float delta);
    void draw();
};

#endif // STREET_H
