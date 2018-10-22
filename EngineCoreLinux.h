///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreLinux.h

#ifndef ENGINECORELINUX_H
#define ENGINECORELINUX_H

#ifndef _WIN32

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include <vector>
#include <string>
#include <sys/time.h>

#include "EngineCoreBase.h"

#define MULTIPLY_TIME       1
#define MAX_DELTA           0.15
#define MIN_DELTA           0.007
#define REAL_INT_MULTIPLY   2

class EngineCore : public EngineCoreBase
{
protected:

    int init();

    virtual ~EngineCore(){};

    void run();

private:
    int prevMouseX;
    int prevMouseY;

    timeval startTime;
    timeval lastTime;

    bool updateRatio;

    Display   *dpy;
    Window     win;
    GLboolean  doubleBuffer;

    long eventMask;

    void initLight();

public:
    static int width;
    static int height;

    static int argc;
    static char **argv;
};

#endif // _WIN32
#endif // ENGINECORE_H

