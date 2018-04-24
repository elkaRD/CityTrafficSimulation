#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"

class Vehicle : public GameObject
{
public:

private:
    float velocity;
    void update(float delta);
protected:

};

class Car : public Vehicle
{
private:

    void update(float delta);
    void draw();
};

#endif // VEHICLE_H
