#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>
#include <cstdlib>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

//#include "Graphics.h"

class Simulator;
class GameObject;
void registerNewObject(Simulator *engine, GameObject *go);

#include "GameObject.h"
#include "Street.h"
#include "Vehicle.h"

#define MULTIPLY_TIME   4
#define MAX_DELTA       0.15

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
class GameObject;
class Simulator
{
    friend GameObject;

public:
    static int init(int argc, char** argv);

    Simulator();
    void loadRoad(std::string fileName);
    void run();
private:
    void update(float delta);
    void registerObject(GameObject *go);
    void destroyObject(GameObject *go);

    std::vector<GameObject*> objects;

    void redraw();

    timeval startTime;
    timeval lastTime;

    static bool didInit;

    static int snglBuf[];// = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    static int dblBuf[];//  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

    static int width;
    static int height;

    static Display   *dpy;
    static Window     win;
    GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;
    static GLboolean  doubleBuffer;// = GL_TRUE;

    static void initLight();

    friend void registerNewObject(Simulator *engine, GameObject *go);
};



//int init(int argc, char** argv);

#endif // SIMULTOR_H
