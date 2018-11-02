///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreWindows.h

#ifndef ENGINECOREWINDOWS_H
#define ENGINECOREWINDOWS_H

#ifdef _WIN32

#include <GL/gl.h>

#include <ctime>
#include <vector>
#include <string>
#include <windows.h>

#include "EngineCoreBase.h"

class EngineCore : protected EngineCoreBase
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
    HWND hwnd;

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
