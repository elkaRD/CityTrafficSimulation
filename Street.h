#ifndef STREET_H
#define STREET_H

#include "GameObject.h"
#include "Vehicle.h"
#include <sstream>

class Cross;
class Vehicle;
class Simulator;

class Road : public GameObject
{
public:
    static Vec3 roadColor;
    float getLength() const;

protected:
    float length;
};

class Driveable : public Road
{
public:
    Vec3 getJointPoint(bool dir) const;
    Vec3 getNormal() const;
    Vec3 getDirection() const;

protected:
    Driveable(Cross *begCross, Cross *endCross);
    Driveable(Vec3 p, Cross *endCross);

    std::queue<Vehicle*> vehiclesBeg;
    std::queue<Vehicle*> vehiclesEnd;

    Vec3 begPos;
    Vec3 endPos;

    Vec3 begJoint;
    Vec3 endJoint;

    Vec3 getBegJointWidth(bool dir) const;
    Vec3 getEndJointWidth(bool dir) const;

    Vec3 direction;
    Vec3 normal;

    virtual float freeSpace(bool dir) const;

    Cross* crossBeg;
    Cross* crossEnd;

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
    virtual void setDefaultPriority(Driveable *s0 = NULL, Driveable *s1 = NULL, Driveable *s2 = NULL, Driveable *s3 = NULL);

protected:

    struct OneStreet
    {
        Driveable *street;
        std::vector<Vehicle*> vehicles;
        bool direction;
        Vec3 getJointPos();

        std::vector<std::vector<int> > yield;
    };
    std::vector<OneStreet> streets;

    virtual void updateCross(float delta);
    virtual bool dontCheckStreet(int which);

    virtual void tryPassVehiclesWithPriority();
    virtual void tryPassAnyVehicle();

    void draw();

private:
    bool isSet;
    int allowedVeh;

    bool checkSet();
    void update(float delta);

    friend Driveable;
    friend Vehicle;
};

class CrossLights : public Cross
{
public:
    CrossLights(Vec3 position);

private:
    struct LightsDuration
    {
        float durationGreen1;
        float durationGreen2;
        float durationYellow1;
        float durationYellow2;
        float durationBreak;
    } durLight;

    std::vector<bool> defaultPriority;
    std::vector<bool> curPriority;

    void setDefaultPriority(Driveable *s0 = NULL, Driveable *s1 = NULL, Driveable *s2 = NULL, Driveable *s3 = NULL);
    void setDefaultLights(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3);
    void setLightsPriority();

    float curTime;

    enum State{G1, Y1, B1, G2, Y2, B2};
    State curState;
    void getNextState();

    bool dontCheckStreet(int which);

    void update(float delta);
    void draw();
};

class Garage : public Driveable
{
public:
    Garage(Vec3 p, Cross *c);

    int vehType;

    bool checkReadyToSpot() const;
    bool checkReadyToDelete() const;

private:
    float frecSpot;
    float curTimeSpot;

    float frecDelete;
    float curTimeDelete;

    void draw();
    void update(float delta);
    std::string itos(int x);
    Vehicle* spotVeh();
    Vehicle* deleteVeh();

    bool isReadyToSpot;
    bool isReadyToDelete;

    friend Simulator;
};

#endif // STREET_H
