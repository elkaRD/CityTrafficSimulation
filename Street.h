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
    static Vec3 roadColor;
    float getLength();

protected:
    float length;
};

class Driveable : public Road
{
public:
    Vec3 getJointPoint(bool dir);
    //Vec3 getBegJoint();
    //Vec3 getEndJoint();
    Vec3 getNormal();
    Vec3 getDirection();

protected:

    Driveable(Cross *begCross, Cross *endCross);
    Driveable(Vec3 p, Cross *endCross);

    std::queue<Vehicle*> vehiclesBeg;
    std::queue<Vehicle*> vehiclesEnd;

    Vec3 begPos;
    Vec3 endPos;

    Vec3 begJoint;
    Vec3 endJoint;

    Vec3 getBegJointWidth(bool dir);
    Vec3 getEndJointWidth(bool dir);

    Vec3 direction;

    Vec3 normal;

    virtual float freeSpace(bool dir);

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
    //Vec3 begPos;
    //Vec3 endPos;

    void draw();


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
    virtual void setDefaultPriority(Driveable *s0 = NULL, Driveable *s1 = NULL, Driveable *s2 = NULL, Driveable *s3 = NULL);

protected:

    struct OneStreet
    {
        Driveable *street;
        std::vector<Vehicle*> vehicles;
        //bool enabled;
        bool direction;
        //Vec3 jointPos;
        Vec3 getJointPos();

        std::vector<std::vector<int> > yield;
    };
    std::vector<OneStreet> streets;





    //virtual void additionalSetup(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3);


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

    float durationGreen1;
    float durationGreen2;
    float durationYellow1;
    float durationYellow2;
    float durationBreak;

    std::vector<bool> defaultPriority;
    std::vector<bool> curPriority;

    void setDefaultPriority(Driveable *s0 = NULL, Driveable *s1 = NULL, Driveable *s2 = NULL, Driveable *s3 = NULL);
    void setDefaultLights(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3);
    void setLightsPriority();

    float curTime;

    enum State{G1, Y1, B1, G2, Y2, B2};
    State curState;
    void getNextState();
    //void additionalSetup(Driveable *s0, Driveable *s1, Driveable *s2, Driveable *s3);

    bool dontCheckStreet(int which);

    void update(float delta);
    void draw();

    //void setDefaultPriority(Road *s0 = NULL, Road *s1 = NULL, Road *s2 = NULL, Road *s3 = NULL);
};

class Garage : public Driveable
{
public:
    Garage(Vec3 p, Cross *c);

    //Cross *cross;
    //std::queue<Vehicle*> vehicles;
    //std::queue<Vehicle*> toDelete;

    int vehType;

    bool checkReadyToSpot();
    bool checkReadyToDelete();

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
