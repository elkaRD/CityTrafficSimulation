#ifndef STREET_H
#define STREET_H

#include "Road.h"

class Street
{
public:

private:
    Vec3 begPos;
    Vec3 endPos;
};

class StreetOneDir : Street
{
private:
    vector<Vehicle> vehicles;
};

class StreetTwoDir : Street
{
private:
    vector<Vehicle> vehicles1;
    vector<Vehicle> vehicles2;
};

#endif // STREET_H
