///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreLinux.h

#ifndef ENGINECOREWINDOWS_H
#define ENGINECOREWINDOWS_H

#ifdef _WIN32

#include <GL/gl.h>

#include <ctime>
#include <vector>
#include <string>
#include <windows.h>

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
    clock_t prevTime;

    int prevMouseX;
    int prevMouseY;


    bool updateRatio;

    HINSTANCE hInstance;
    HDC hDC;

    //LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
    void DisableOpenGL(HWND, HDC, HGLRC);

    void initLight();

    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

public:
    static int width;
    static int height;

    static int argc;
    static char **argv;
};

#endif // _WIN32
#endif // ENGINECOREWINDOWS_H
