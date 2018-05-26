#ifndef SIMULATOR_H
#define SIMULATOR_H

#include"EngineCore.h"

//#include "Graphics.h"

class Simulator;
class GameObject;
void registerNewObject(Simulator *engine, GameObject *go);
void destroyNextObject(Simulator *engine, GameObject *go);

#include "GameObject.h"
#include "Street.h"
#include "Vehicle.h"




class GameObject;



class Simulator : public EngineCore
{
    friend GameObject;

public:
    //static int init(int argc, char** argv);

    Simulator();
    void loadRoad(std::string fileName);
    void loadPriority(std::string fileName);
    //void run();

    Vec3 cameraPos;
    Vec3 cameraRot;
private:
    //void update(float delta);
    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

    void moveCamera(float delta);
    void moveCamera2(char c);
    bool isKeyPressed(long k);

    GameObject* findObjectByName(std::string on);

    std::vector<GameObject*> objects;

    //void redraw();

    //void run();

    void keyPressed(char k);
    void update(float delta);
    void redraw();
    void mouseMove(int dx, int dy);

    friend void registerNewObject(Simulator *engine, GameObject *go);
    friend void destroyNextObject(Simulator *engine, GameObject *go);
};



//int init(int argc, char** argv);

#endif // SIMULTOR_H
