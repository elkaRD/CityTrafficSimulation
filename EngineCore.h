#ifndef ENGINECORE_H
#define ENGINECORE_H

#include <cstdio>
#include <cstdlib>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#define MULTIPLY_TIME       1
#define MAX_DELTA           0.15
#define MIN_DELTA           0.007
#define REAL_INT_MULTIPLY   2
#define CAMERA_VELOCITY     1

class EngineCore
{
public:

    static int init(int argc, char **argv);
    static bool didInit;

    virtual ~EngineCore(){};

    void run();

    virtual void keyPressed(char k) = 0;
    virtual void update(float delta) = 0;
    virtual void redraw() = 0;
    virtual void mouseMove(int dx, int dy) = 0;

    int prevMouseX;
    int prevMouseY;

    timeval startTime;
    timeval lastTime;

    static int snglBuf[];// = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    static int dblBuf[];//  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

    static int width;
    static int height;
    static bool updateRatio;

    bool pressedKey[256];
    bool pressedShift;

    static Display   *dpy;
    static Window     win;
    //GLfloat    xAngle = 0.0, yAngle = 82.0, zAngle = 112.0;
    static GLboolean  doubleBuffer;// = GL_TRUE;
    //static XSetWindowAttributes swa;
    static long eventMask;

    static void initLight();
};

#endif // ENGINECORE_H
