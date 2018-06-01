///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCore.h


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

class EngineCore
{
protected:

    int init(int argc, char **argv);
    static bool didInit;

    virtual ~EngineCore(){};

    void run();

    virtual void keyPressed(char k) = 0;
    virtual void update(float delta) = 0;
    virtual void singleUpdate(float delta) = 0;
    virtual void redraw() = 0;
    virtual void mouseMove(int dx, int dy) = 0;

private:
    int prevMouseX;
    int prevMouseY;

    timeval startTime;
    timeval lastTime;

    bool updateRatio;

    //bool pressedKey[256];
    //bool pressedShift;

    Display   *dpy;
    Window     win;
    GLboolean  doubleBuffer;
    //static XSetWindowAttributes swa;
    long eventMask;

    void initLight();

public:
    static int width;
    static int height;
};

#endif // ENGINECORE_H
