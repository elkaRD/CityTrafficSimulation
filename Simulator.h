///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Simulator.h


#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "EngineCore.h"
#include "Graphics.h"
#include "ObjectsLoader.h"

class GameObject;

class Simulator : private EngineCore, private Graphics, public ObjectsLoader
{
    friend GameObject;

public:
    static Simulator &getInstance();

    Vec3 cameraPos;
    Vec3 cameraRot;

    void run();

protected:
    GameObject* findObjectByName(const std::string objectName) const;
    void LoadedNewObject(GameObject *newGameObject);
    void LoadedNewFactory(Garage *newFactory);

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
        DOWN
    };

    unsigned int cameraDirection;
    float cameraVelocity;

    void cameraMove(const float delta);

    const float CAMERA_VELOCITY;
};

#endif // SIMULTOR_H
