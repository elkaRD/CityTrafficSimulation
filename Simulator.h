#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>
#include <cstdlib>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "GameObject.h"
#include <vector>

class Simulator
{
public:
    static int init(int argc, char** argv);

    Simulator();
    void loadRoad();
    void run();
private:
    void update(float delta);
    void registerObject(GameObject go);
    void destroyObject(GameObject go);
    std::vector<GameObject*> objects;


    static bool didInit;
};

//int init(int argc, char** argv);

#endif // SIMULTOR_H
