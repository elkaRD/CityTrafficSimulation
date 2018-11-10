///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Simulator.cpp

#include"Simulator.h"
using namespace std;

Simulator &Simulator::getInstance()
{
    static Simulator instanceSimulator;

    return instanceSimulator;
}

void Simulator::run()
{
    objects.reserve(maxNumberOfObjects);
    EngineCore::run();
}

void Simulator::redraw()
{
    rotateX(cameraRot.y);
    rotateY(cameraRot.x);

    scale(10, 10, 10);

    translate(-cameraPos);

    scale(1, 1, -1);

    pushMatrix();

    for (const auto &object : objects)
    {
        object->drawObject();
    }

    popMatrix();
}

Simulator::Simulator() : maxNumberOfObjects(0), CAMERA_VELOCITY(3)
{
    init();

    cameraPos = Vec3(-5.5, 2.5, -7.84);
    cameraRot = Vec3(-215, 13.2, 0);

    cameraDirection = 0;
}

void Simulator::keyPressed(char k)
{
    if (k == 27)
    {
        cleanSimulation();
        exit(0);
    }

    if (k >= 'A' && k <= 'Z')
    {
        k += 32;
        cameraVelocity = CAMERA_VELOCITY * 5.0f;
    }
    else
    {
        cameraVelocity = CAMERA_VELOCITY;
    }

    switch (k)
    {
        case 'w': cameraDirection |= (1 << FORWARD);    break;
        case 's': cameraDirection |= (1 << BACK);       break;
        case 'a': cameraDirection |= (1 << LEFT);       break;
        case 'd': cameraDirection |= (1 << RIGHT);      break;
        case 'e': cameraDirection |= (1 << UP);         break;
        case 'q': cameraDirection |= (1 << DOWN);       break;
    }
}

void Simulator::cameraMove(const float delta)
{
    float multiplyMove = cameraVelocity * delta;
    float multiplyMoveHorizontal = cos(-cameraRot.y * M_PI / 180) * multiplyMove;

    if ((cameraDirection >> FORWARD) & 1)
    {
        cameraPos.z -=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x +=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y)*M_PI/180)*multiplyMove;
    }

    if ((cameraDirection >> BACK) & 1)
    {
        cameraPos.z +=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x -=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y -=sin((-cameraRot.y)*M_PI/180)*multiplyMove;
    }

    if ((cameraDirection >> LEFT) & 1)
    {
        cameraPos.z +=cos( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        cameraPos.x -=sin( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
    }

    if ((cameraDirection >> RIGHT) & 1)
    {
        cameraPos.z -=cos( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
        cameraPos.x +=sin( (cameraRot.x + 90.0) * M_PI / 180 )*multiplyMove;
    }

    if ((cameraDirection >> UP) & 1)
    {
        cameraPos.z -=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x +=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y + 90)*M_PI/180)*multiplyMove;
    }

    if ((cameraDirection >> DOWN) & 1)
    {
        cameraPos.z +=cos( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.x -=sin( cameraRot.x * M_PI / 180 )*multiplyMoveHorizontal;
        cameraPos.y +=sin((-cameraRot.y - 90)*M_PI/180)*multiplyMove;
    }
    cameraDirection = 0;
}

void Simulator::mouseMove(const int dx, const int dy)
{
    cameraRot.x += dx * 0.2;
    cameraRot.y += dy * 0.2;

    if (cameraRot.y > 90) cameraRot.y = 90;
    else if (cameraRot.y < -90) cameraRot.y = -90;
}


void Simulator::singleUpdate(const float delta)
{
    cameraMove(delta);
}

void Simulator::update(const float delta)
{
    for (auto &spot : spots)
    {
        if (spot->checkReadyToSpot())
        {
            registerObject(spot->spotVeh());
        }
        if (spot->checkReadyToDelete())
        {
            destroyObject(spot->deleteVeh());
        }
    }

    for (auto &object : objects)
    {
        object->updateObject(delta);
    }
}

void Simulator::registerObject(GameObject *go)
{
    objects.push_back(go);
}

void Simulator::destroyObject(GameObject *go)
{
     auto objectToRemove = find_if(objects.begin(), objects.end(), [&go] (GameObject *item) {return item == go;});
     iter_swap(objectToRemove, objects.end() - 1);
     objects.pop_back();
}

void Simulator::cleanSimulation()
{
    int i = objects.size();

    while (i > 0)
    {
        destroyObject(*objects.begin());
        i--;
    }
}

GameObject* Simulator::findObjectByName(const string objectName) const
{
    auto foundObject = find_if(objects.begin(), objects.end(), [&objectName] (GameObject *item) {return item->id.compare(objectName) == 0;} );

    if (foundObject != objects.end()) return *foundObject;
    return nullptr;
}

void Simulator::loadedNewObject(GameObject *newGameObject)
{
    objects.push_back(newGameObject);
    maxNumberOfObjects++;
}

void Simulator::loadedNewFactory(Garage *newFactory)
{
    spots.push_back(newFactory);
    maxNumberOfObjects += newFactory->maxVehicles;
}
