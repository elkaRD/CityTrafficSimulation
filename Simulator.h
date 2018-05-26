#ifndef SIMULATOR_H
#define SIMULATOR_H

#include"EngineCore.h"

class Simulator;
class GameObject;

#include "GameObject.h"
#include "Street.h"
#include "Vehicle.h"

class GameObject;

class Simulator : public EngineCore
{
    friend GameObject;

public:
    Simulator();

    void loadRoad(std::string fileName);
    void loadPriority(std::string fileName);

    Vec3 cameraPos;
    Vec3 cameraRot;

private:
    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

    void moveCamera(float delta);
    void moveCamera2(char c);
    bool isKeyPressed(long k);

    GameObject* findObjectByName(std::string on);

    std::vector<GameObject*> objects;

    void keyPressed(char k);
    void update(float delta);
    void redraw();
    void mouseMove(int dx, int dy);

    //static void registerNewObject(Simulator *engine, GameObject *go);
    //static void destroyNextObject(Simulator *engine, GameObject *go);

    friend void registerNewObject(Simulator *engine, GameObject *go);
    friend void destroyNextObject(Simulator *engine, GameObject *go);
};

#endif // SIMULTOR_H
