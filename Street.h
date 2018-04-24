#ifndef STREET_H
#define STREET_H

//#include "Road.h"
#include "GameObject.h"
#include "Vehicle.h"

class Cross;

class Road : public GameObject
{

};

class Street : public Road
{
public:
    Street(Cross *begCross, Cross *endCross);

private:
    //Vec3 begPos;
    //Vec3 endPos;

    Cross* crossBeg;
    Cross* crossEnd;

    std::queue<Vehicle> vehiclesBeg;
    std::queue<Vehicle> vehiclesEnd;
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
        Street street;
        std::queue<Vehicle> vehicles;
    };
    std::vector<OneStreet> streets;
};

#endif // STREET_H
