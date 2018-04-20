#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"

class Vehicle : GameObject
{
public:

private:
    float velocity;
    void update() override;
protected:

};

class Car : Vehicle
{
private:

    void draw() override;
};

#endif // VEHICLE_H
