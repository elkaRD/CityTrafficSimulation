#ifndef OBJECTMANAGERBRIDGE_H
#define OBJECTMANAGERBRIDGE_H

class Simulator;
class GameObject;

class ObjectManagerBridge
{
    static void registerNewObject(Simulator *engine, GameObject *go);
    static void destroyNextObject(Simulator *engine, GameObject *go);
};

#endif // OBJECTMANAGERBRIDGE_H
