///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreWindows.cpp

//klasa silnika gry jest oparta na przykladowym projekcie OpenGL z Code::Blocks'a (OpenGL + X11)

#ifdef _WIN32

#include "EngineCoreWindows.h"
using namespace std;

void fatalError(string e)
{
    throw "ENGINE CORE ERROR: " + e;
}

//int EngineCore::argc = 0;
//char **EngineCore::argv = NULL;
EngineCore *EngineCore::instance = nullptr;

void EngineCore::SetCmdArgs(int argC, char **argV)
{

}

int EngineCore::init()
{
    instance = this;

    width = 1280;
    height = 720;

    hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wcex;
    HWND hwnd;

    HGLRC hRC;
    //MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "City traffic",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0);
    initLight();

    return 0;
}

void EngineCore::CheckKeyboard()
{
    for (int i=0; i < 128; i++)
    {
        if (GetAsyncKeyState(i) != 0) keyPressed(i);
    }
}

void EngineCore::CheckMouse()
{
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    if((GetKeyState(VK_LBUTTON) & 0x100) != 0)
        mouseMove(cursorPos.x - prevMouseX, cursorPos.y - prevMouseY);

    prevMouseX = cursorPos.x;
    prevMouseY = cursorPos.y;
}

void EngineCore::run()
{
    MSG msg;

    while (true)
    {
        /* check for messages */
        CheckKeyboard();
        CheckMouse();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {


        clock_t newTime = clock();
        float delta = clock() - newTime;
        delta /= CLOCKS_PER_SEC;
        prevTime = newTime;

        delta *= MULTIPLY_TIME;
        if (delta > MAX_DELTA * MULTIPLY_TIME) delta = MAX_DELTA * MULTIPLY_TIME;
        if (delta < MIN_DELTA * MULTIPLY_TIME) delta = MIN_DELTA * MULTIPLY_TIME;
        if (delta > 0.4) delta = 0.4;

        updateRatio = true; //todo: temp solution; fix in the future
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

        for(int i=0;i<REAL_INT_MULTIPLY;i++)
        {
            update(delta);
        }

        redraw();

        SwapBuffers(hDC);
        glFlush();

        }
    }
}
#include<iostream>
using namespace std;
LRESULT CALLBACK EngineCore::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;

                //default:
                    //Simulator::getInstance().keyPressed(wParam);
                    //GetBaseInstance()->keyPressed(wParam);
                   //instance->keyPressed(wParam);
            }
        }
        break;

                case WM_SIZE:
                    {
                        instance->width = (int)LOWORD(lParam);
                        instance->height = (int)HIWORD(lParam);

                        cout << instance->height<<"  x  "<<instance->width<<endl;

                        break;
                    }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EngineCore::EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void EngineCore::DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

#endif // _WIN32
