///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
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

class EngineCore : protected EngineCoreBase
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

