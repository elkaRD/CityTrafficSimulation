#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"

class Vehicle : public GameObject
{
public:

private:
    float velocity;
    void update();
protected:

};

class Car : public Vehicle
{
private:

    void draw() override;
};

#endif // VEHICLE_H
