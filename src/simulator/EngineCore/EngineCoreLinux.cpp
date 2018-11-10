///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreLinux.cpp

//This class bases on Code::Blocks OpenGL template (OpenGL + X11)

#ifndef _WIN32

#include "EngineCoreLinux.h"
using namespace std;

int EngineCore::argc = 0;
char **EngineCore::argv = NULL;

void EngineCore::SetCmdArgs(int argC, char **argV)
{
    argc = argC;
    argv = argV;
}

int EngineCore::init()
{
    int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

    dpy = NULL;
    win = (Window) NULL;
    doubleBuffer = GL_TRUE;
    eventMask = 0;

    XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;

    int                  dummy;

    /*** (1) open a connection to the X server ***/

    dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
        throw ExceptionClass("could not open display");

    /*** (2) make sure OpenGL's GLX extension supported ***/

    if(!glXQueryExtension(dpy, &dummy, &dummy))
        throw ExceptionClass("X server has no OpenGL GLX extension");

    /*** (3) find an appropriate visual ***/

    /* find an OpenGL-capable RGB visual with depth buffer */
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) throw ExceptionClass("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }
    if(vi->c_class != TrueColor)
        throw ExceptionClass("TrueColor visual required for this program");

    /*** (4) create an OpenGL rendering context  ***/

    /* create an OpenGL rendering context */
    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                            /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL)
        throw ExceptionClass("could not create rendering context");

    /*** (5) create an X window with the selected visual ***/

    /* create an X colormap since probably not using default visual */
    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask    | ExposureMask | KeyReleaseMask
                     | ButtonPressMask | StructureNotifyMask | Button1MotionMask;
    eventMask = swa.event_mask;
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                          width, height, 0, vi->depth, InputOutput, vi->visual,
                          CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "City traffic simulation", "main", None, argv, argc, NULL);

    /*** (6) bind the rendering context to the window ***/

    glXMakeCurrent(dpy, win, cx);

    XAutoRepeatOff(dpy);

    /*** (8) configure the OpenGL context for rendering ***/

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0);

    initLight();

    gettimeofday(&startTime, 0);
    lastTime = startTime;

    for (int i = 0; i < 256; i++)
        heldKeys[i] = false;

    return 0;
}

void EngineCore::showWindow()
{
    XMapWindow(dpy, win);
}

void EngineCore::hideWindow()
{
    XUnmapWindow(dpy, win);
}

float EngineCore::getDeltaTime()
{
    timeval newTime;
    gettimeofday(&newTime, 0);
    int secB = newTime.tv_sec * 1000000 + newTime.tv_usec;
    int secE = lastTime.tv_sec * 1000000 + lastTime.tv_usec;
    float delta = secB - secE;
    delta /= 1000000.0;

    lastTime = newTime;

    return delta;
}

void EngineCore::checkEvents()
{
    XEvent event;

    while (XPending(dpy))
    {
        XCheckWindowEvent(dpy, win, eventMask, &event);

        switch (event.type)
        {
            case KeyPress:
            {
                KeySym     keysym;
                char       buffer[4];
                XLookupString((XKeyEvent *)&event, buffer, 4, &keysym, NULL);

                keyPressed(buffer[0]);
                heldKeys[(unsigned int)buffer[0]] = true;

                break;
            }

            case KeyRelease:
            {
                KeySym     keysym;
                char       buffer[4];
                XLookupString((XKeyEvent *)&event, buffer, 4, &keysym, NULL);

                heldKeys[(unsigned int)buffer[0]] = false;
                if (buffer[0] >= 'a' && buffer[0] <= 'z') heldKeys[(unsigned int)buffer[0] - 32] = false;
                if (buffer[0] >= 'A' && buffer[0] <= 'Z') heldKeys[(unsigned int)buffer[0] + 32] = false;
                break;
            }

            case ButtonPress:

                XButtonEvent *bevent;
                bevent = (XButtonEvent*) &event;

                prevMouseX = bevent->x;
                prevMouseY = bevent->y;

                break;

            case MotionNotify:
            {
                XMotionEvent *mevent;
                mevent = (XMotionEvent *) &event;

                int dx = mevent->x - prevMouseX;
                int dy = mevent->y - prevMouseY;

                mouseMove(dx, dy);

                prevMouseX = mevent->x;
                prevMouseY = mevent->y;

                break;
            }

            case ConfigureNotify:

                width = event.xconfigure.width;
                height = event.xconfigure.height;
                updateRatio();

                break;
        }
    }

    for (int i = 0; i < 256; i++)
        if (heldKeys[i])
            keyHeld(i);
}

void EngineCore::swapBuffers()
{
    if (doubleBuffer)
        glXSwapBuffers(dpy, win);
}

#endif // _WIN32
