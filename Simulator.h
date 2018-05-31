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

    void loadRoad(std::string fileName);
    void loadPriority(std::string fileName);

    Vec3 cameraPos;
    Vec3 cameraRot;

private:
    static Simulator *instance;
    Simulator();

    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

    GameObject* findObjectByName(std::string on) const;

    std::vector<GameObject*> objects;
    std::vector<Garage*> spots;

    void keyPressed(char k);
    void update(float delta);
    void singleUpdate(float delta);
    void redraw();
    void mouseMove(int dx, int dy);

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

    void cameraMove(float delta);
};

#endif // SIMULTOR_H
