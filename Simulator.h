///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Simulator.h


#ifndef SIMULATOR_H
#define SIMULATOR_H

#include"EngineCore.h"

class Simulator;
class GameObject;

#include "GameObject.h"
#include "Street.h"
#include "Vehicle.h"

#define CAMERA_VELOCITY     3

class GameObject;

class Simulator : public EngineCore
{
    friend GameObject;

public:
    static Simulator *getInstance();

    void loadRoad(const std::string fileName);
    void loadPriority(const std::string fileName);

    Vec3 cameraPos;
    Vec3 cameraRot;

private:
    static Simulator *instance;
    Simulator();

    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

    GameObject* findObjectByName(const std::string on) const;

    std::vector<GameObject*> objects;
    std::vector<Garage*> spots;

    void keyPressed(char k);
    void update(const float delta);
    void singleUpdate(const float delta);
    void redraw();
    void mouseMove(const int dx, const int dy);

    enum DirectionMove
    {
        FORWARD,
        BACK,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        STAY
    };

    DirectionMove cameraDirection;
    float cameraVelocity;

    void cameraMove(const float delta);
};

#endif // SIMULTOR_H
