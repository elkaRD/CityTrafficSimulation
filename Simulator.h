///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Simulator.h


#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fstream>
#include <cmath>
#include <algorithm>

#define M_PI 3.14159265358979323846

#include "EngineCore.h"
#include "Graphics.h"
#include "GameObject.h"
#include "Street.h"
#include "Vehicle.h"

//#define CAMERA_VELOCITY     3

class GameObject;

class Simulator : public EngineCore, public Graphics
{
    friend GameObject;

public:
    static Simulator &getInstance();

    void loadRoad(const std::string fileName);
    void loadPriority(const std::string fileName);

    Vec3 cameraPos;
    Vec3 cameraRot;

    void run();

    GameObject* findObjectByName(const std::string on) const;

private:
    Simulator();

    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

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

    unsigned int cameraDirection;
    float cameraVelocity;

    void cameraMove(const float delta);

    const float CAMERA_VELOCITY;
};

#endif // SIMULTOR_H
