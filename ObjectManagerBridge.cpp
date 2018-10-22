#include"ObjectManagetBridge.h"

void ObjectManagerBridge::registerNewObject(Simulator *engine, GameObject *go)
{
    engine->registerObject(go);
}

void ObjectManagerBridge::destroyNextObject(Simulator *engine, GameObject *go)
{
    engine->destroyObject(go);
}
