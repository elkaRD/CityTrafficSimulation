///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Garage.cpp


#include "Garage.h"
using namespace std;

int Garage::vehiclesCounter = 0;

Garage::Garage(Vec3 p, Cross *c) : Driveable(p, c)
{
    curTimeSpot = 0;
    frecSpot = 4;

    curTimeDelete = 0;
    frecDelete = 6;

    isReadyToSpot = false;
    isReadyToDelete = false;

    spottedVehicles = 0;
    maxVehicles = 30;
}

void Garage::draw()
{
    setColor(0.5, 0 ,0);
    pushMatrix();
    translate(0,0.2,0);
    rotateY(direction.angleXZ());
    drawCube(0.7, 0.4, 1);
    popMatrix();

    translate(-pos);
    Driveable::draw();
}

void Garage::update(const float delta)
{
    if (vehiclesBeg.size() == 0 || (vehiclesBeg.size() > 0 && vehiclesBeg.back()->xPos > 1))
        curTimeSpot += delta;

    if (curTimeSpot > frecSpot && spottedVehicles < maxVehicles)
    {
        isReadyToSpot = true;
    }

    if (vehiclesEnd.size() > 0)
        curTimeDelete += delta;

    if (curTimeDelete > frecDelete)
    {
        if (vehiclesEnd.size() > 0)
        {
            isReadyToDelete = true;
        }
    }
}

string Garage::itos(const int x)
{
    ostringstream ss;
    ss << x;
    return ss.str();
}

Vehicle* Garage::spotVeh()
{
    curTimeSpot = 0;
    isReadyToSpot = false;

    Vehicle *temp = createVehicle();

    temp->curRoad = this;
    temp->direction = true;
    temp->isFirstVeh = vehiclesBeg.size() == 0;

    vehiclesBeg.push(temp);

    spottedVehicles++;
    vehiclesCounter++;

    return temp;
}

Vehicle* Garage::deleteVeh()
{
    curTimeDelete = 0;
    isReadyToDelete = false;

    if (vehiclesEnd.size() > 0)
    {
        Vehicle *temp = vehiclesEnd.front();
        vehiclesEnd.pop();

        if(temp->backVeh != nullptr)
        {
            temp->backVeh->isFirstVeh = true;
            temp->backVeh->frontVeh = nullptr;
        }
        delete temp;

        spottedVehicles--;

        return temp;
    }

    return nullptr;
}

bool Garage::checkReadyToSpot() const
{
    return isReadyToSpot;
}

bool Garage::checkReadyToDelete() const
{
    return isReadyToDelete;
}

GarageCar::GarageCar(Vec3 p, Cross *c) : Garage(p, c)
{

}

Vehicle *GarageCar::createVehicle()
{
    Vehicle *temp = new Car(this);
    temp->id = "CAR_" + id + "_" + itos(Garage::vehiclesCounter);
    return temp;
}

GarageBus::GarageBus(Vec3 p, Cross *c) : Garage(p, c)
{

}

Vehicle *GarageBus::createVehicle()
{
    Vehicle *temp = new Bus(this);
    temp->id = "BUS_" + id + "_" + itos(Garage::vehiclesCounter);
    return temp;
}
