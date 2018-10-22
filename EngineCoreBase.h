///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreBase.h


#ifndef ENGINECOREBASE_H
#define ENGINECOREBASE_H

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include <vector>
#include <string>
#include <sys/time.h>

#define MULTIPLY_TIME       1
#define MAX_DELTA           0.15
#define MIN_DELTA           0.007
#define REAL_INT_MULTIPLY   2

class EngineCoreBase
{
protected:

    //int init(int argc, char **argv);
    virtual int init() = 0;
    static bool didInit;

    virtual ~EngineCoreBase(){};

    virtual void run() = 0;

    virtual void keyPressed(char k) = 0;
    virtual void update(float delta) = 0;
    virtual void singleUpdate(float delta) = 0;
    virtual void redraw() = 0;
    virtual void mouseMove(int dx, int dy) = 0;

private:

    void initLight();

public:
    static int width;
    static int height;
};

#endif // ENGINECORE_H
