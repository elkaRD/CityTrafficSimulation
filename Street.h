#ifndef STREET_H
#define STREET_H

//#include "Road.h"
#include "GameObject.h"
#include "Vehicle.h"
#include <sstream>

class Cross;
class Vehicle;
//class GameObject;
class Simulator;
//void Simulator::registerObject(GameObject *go);

class Road : public GameObject
{
public:

    std::queue<Vehicle*> vehiclesBeg;
    std::queue<Vehicle*> vehiclesEnd;

    Vec3 begPos;
    Vec3 endPos;

    Vec3 begJoint;
    Vec3 endJoint;

    Vec3 getBegJoint(bool dir);
    Vec3 getEndJoint(bool dir);

    float length;
    Vec3 direction;

    Vec3 normal;

    virtual float freeSpace(bool dir);

    float reservedSpaceBeg;
    float reservedSpaceEnd;

    Cross* crossBeg;
    Cross* crossEnd;
};

class Street : public Road
{
public:
    Street(Cross *begCross, Cross *endCross);

    void draw();

private:
    //Vec3 begPos;
    //Vec3 endPos;




};

/*class StreetOneDir : Street
{
private:
    vector<Vehicle> vehicles;
};

class StreetTwoDir : Street
{
private:
    vector<Vehicle> vehicles1;
    vector<Vehicle> vehicles2;
};*/

class Cross : public Road
{
    //std::vector<Street> streets;
    //std::vector<std::vector<Vehicle>>
public:
    Cross(Vec3 position);

    struct OneStreet
    {
        Road *street;
        std::vector<Vehicle*> vehicles;
        bool enabled = true;
        bool direction;
        //Vec3 jointPos;

        std::vector<std::vector<int> > yield;
    };
    std::vector<OneStreet> streets;

    virtual void setDefaultPriority(Road *s0 = NULL, Road *s1 = NULL, Road *s2 = NULL, Road *s3 = NULL);

    int allowedVeh;
    bool isSet;

    void update(float delta);

    void draw();
};

class CrossLights : public Cross
{
public:
    CrossLights(Vec3 position);

    float durationGreen1;
    float durationGreen2;
    float durationYellow1;
    float durationYellow2;
    float durationBreak;

    float curTime;

    enum State{G1, Y1, B1, G2, Y2, B2};
    State curState;
    void getNextState();

    void update(float delta);
    void draw();

    void setDefaultPriority(Road *s0 = NULL, Road *s1 = NULL, Road *s2 = NULL, Road *s3 = NULL);
};

class Garage : public Road
{
public:
    Garage(Simulator *engine, Vec3 p, Cross *c);

    //Cross *cross;
    //std::queue<Vehicle*> vehicles;
    //std::queue<Vehicle*> toDelete;

    float frecSpot;
    float curTimeSpot;

    float frecDelete;
    float curTimeDelete;

    void draw();
    void update(float delta);
    std::string itos(int x);
    void spotCar();
    void deleteCar();
};

#endif // STREET_H
