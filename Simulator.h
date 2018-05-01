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

#define MULTIPLY_TIME       1
#define MAX_DELTA           0.15
#define REAL_INT_MULTIPLY   3
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
class Simulator
{
    friend GameObject;

public:
    static int init(int argc, char** argv);

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

    void moveCamera(float delta);
    void moveCamera2(char c);
    bool isKeyPressed(long k);

    GameObject* findObjectByName(std::string on);

    bool pressedKey[256];
    bool pressedShift;

    int prevMouseX;
    int prevMouseY;

    std::vector<GameObject*> objects;

    void redraw();

    timeval startTime;
    timeval lastTime;

    static bool didInit;

    static int snglBuf[];// = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    static int dblBuf[];//  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

    static int width;
    static int height;
    bool updateRatio;

    static Display   *dpy;
    static Window     win;
    GLfloat    xAngle = 0.0, yAngle = 82.0, zAngle = 112.0;
    static GLboolean  doubleBuffer;// = GL_TRUE;
    //static XSetWindowAttributes swa;
    static long eventMask;

    static void initLight();

    friend void registerNewObject(Simulator *engine, GameObject *go);
};



//int init(int argc, char** argv);

#endif // SIMULTOR_H
