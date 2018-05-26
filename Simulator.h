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

#define MULTIPLY_TIME       1
#define MAX_DELTA           0.15
#define MIN_DELTA           0.007
#define REAL_INT_MULTIPLY   2
#define CAMERA_VELOCITY     1

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
class GameObject;



class Simulator : public EngineCore
{
    friend GameObject;

public:
    //static int init(int argc, char** argv);

    Simulator();
    void loadRoad(std::string fileName);
    void loadPriority(std::string fileName);
    void run();

    Vec3 cameraPos;
    Vec3 cameraRot;
private:
    void update(float delta);
    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    void cleanSimulation();

    void moveCamera(float delta);
    void moveCamera2(char c);
    bool isKeyPressed(long k);

    GameObject* findObjectByName(std::string on);

    std::vector<GameObject*> objects;

    void redraw();

    friend void registerNewObject(Simulator *engine, GameObject *go);
    friend void destroyNextObject(Simulator *engine, GameObject *go);
};



//int init(int argc, char** argv);

#endif // SIMULTOR_H
