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

class EngineCore : public EngineCoreBase
{
protected:

    int init();
    virtual ~EngineCore(){};

    float getDeltaTime();
    void checkEvents();
    void swapBuffers();

private:
    clock_t prevTime;

    int prevMouseX;
    int prevMouseY;

    HDC hDC;

    void enableOpenGL(HWND hwnd, HDC*, HGLRC*);
    void disableOpenGL(HWND, HDC, HGLRC);

    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

    void checkKeyboard();
    void checkMouse();

    static EngineCore *instance;

public:

    static void SetCmdArgs(int argC, char **argV) {};
};

#endif // _WIN32
#endif // ENGINECOREWINDOWS_H
