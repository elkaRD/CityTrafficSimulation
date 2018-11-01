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

#include"EngineCoreLinux.h"
using namespace std;

int EngineCore::width = 1280;
int EngineCore::height = 720;

void fatalError(string e)
{
    throw "ENGINE CORE ERROR: " + e;
}

int EngineCore::argc = 0;
char **EngineCore::argv = NULL;

int EngineCore::init()
{
    updateRatio = true;

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
        fatalError("could not open display");

    /*** (2) make sure OpenGL's GLX extension supported ***/

    if(!glXQueryExtension(dpy, &dummy, &dummy))
        fatalError("X server has no OpenGL GLX extension");

    /*** (3) find an appropriate visual ***/

    /* find an OpenGL-capable RGB visual with depth buffer */
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) fatalError("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }
    if(vi->c_class != TrueColor)
        fatalError("TrueColor visual required for this program");

    /*** (4) create an OpenGL rendering context  ***/

    /* create an OpenGL rendering context */
    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                            /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL)
        fatalError("could not create rendering context");

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
    XSetStandardProperties(dpy, win, "main", "main", None,
                          argv, argc, NULL);

    /*** (6) bind the rendering context to the window ***/

    glXMakeCurrent(dpy, win, cx);

    /*** (7) request the X window to be displayed on the screen ***/

    XMapWindow(dpy, win);

    /*** (8) configure the OpenGL context for rendering ***/

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0);

    initLight();

    return 0;
}

void EngineCore::initLight()
{
    const GLfloat lambient[]  = { 0.3,0.3,0.3, 1.0f };
    const GLfloat ldiffuse[]  = { 1,1,1, 1.0f };
    const GLfloat lspecular[] = { 0,0,0, 1.0f };
    const GLfloat lposition[] = { 30,30,30, 0.0f };

    const GLfloat mambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mdiffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mspecular[]   = { 0,0,0, 1.0f };
    const GLfloat shininess[] = { 100 };

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  lambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mspecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void EngineCore::run()
{
    XEvent event;

    gettimeofday(&startTime, 0);
    lastTime = startTime;

    while (1)
    {
        XCheckWindowEvent(dpy, win, eventMask, &event);

        switch (event.type)
        {
            case KeyPress:
            {
                KeySym     keysym;
                //XKeyEvent *kevent;
                char       buffer[4];

                //kevent = (XKeyEvent *) &event;
                XLookupString((XKeyEvent *)&event,buffer,4,&keysym,NULL);
                keyPressed(buffer[0]);

                break;
            }

            case KeyRelease:
            {
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

                updateRatio = true;
                width = event.xconfigure.width;
                height = event.xconfigure.height;
                break;
        }

        timeval newTime;
        gettimeofday(&newTime, 0);
        int secB = newTime.tv_sec * 1000000 + newTime.tv_usec;
        int secE = lastTime.tv_sec * 1000000 + lastTime.tv_usec;
        float delta = secB - secE;
        delta /= 1000000.0;

        delta *= MULTIPLY_TIME;
        if (delta > MAX_DELTA * MULTIPLY_TIME) delta = MAX_DELTA * MULTIPLY_TIME;
        if (delta < MIN_DELTA * MULTIPLY_TIME) delta = MIN_DELTA * MULTIPLY_TIME;
        if (delta > 0.4) delta = 0.4;

        lastTime = newTime;

        if (updateRatio)
        {
            updateRatio = false;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            float screenRatio = width / height * 2.0;
            glViewport(0,0,width,height);
            glFrustum(-1.0 * screenRatio, 1.0 * screenRatio, -1.0, 1.0, 5.0, 1000.0);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glTranslatef(0,0,5);

        singleUpdate(delta);

        for(int i=0;i<updatesPerFrame;i++)
        {
            update(delta);
        }

        redraw();

        if (doubleBuffer)
            glXSwapBuffers(dpy, win);
        else
            glFlush();
    }
}

#endif // _WIN32
