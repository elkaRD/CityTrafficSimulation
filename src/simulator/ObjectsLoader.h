///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: ObjectsLoader.h


#ifndef OBJECTSLOADER_H
#define OBJECTSLOADER_H

#include <fstream>
#include <vector>
#include <algorithm>

#include "EngineCore/ExceptionClass.h"
#include "GameObject.h"
#include "Road.h"
#include "Garage.h"
#include "Vehicle.h"

class ObjectsLoader
{
public:
    void loadRoad(const std::string fileName);
    void loadRightOfWay(const std::string fileName);

protected:
    virtual GameObject* findObjectByName(const std::string on) const = 0;
    virtual void loadedNewObject(GameObject *newGameObject) = 0;
    virtual void loadedNewFactory(Garage *newIntersection) = 0;

private:
    enum EmptyLineException{};
};

#endif // OBJECTSLOADER_H
