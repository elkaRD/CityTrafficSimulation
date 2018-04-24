#ifndef STREET_H
#define STREET_H

#include "Road.h"

class Cross;

class Street
{
public:

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

class Cross
{
    //std::vector<Street> streets;
    //std::vector<std::vector<Vehicle>>
    struct OneStreet
    {
        Street street;
        std::queue<Vehicle> vehicles;
    };
    std::vector<OneStreet> streets;
};

#endif // STREET_H
