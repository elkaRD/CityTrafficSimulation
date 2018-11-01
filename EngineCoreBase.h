///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreBase.h


#ifndef ENGINECOREBASE_H
#define ENGINECOREBASE_H

#include <GL/gl.h>

#include <vector>
#include <string>

class EngineCoreBase
{
protected:
    float timeScale;
    int updatesPerFrame;

    const float MIN_TIME_SCALE;
    const float MAX_TIME_SCALE;

    const int MIN_UPDATES_PER_FRAME;
    const int MAX_UPDATES_PER_FRAME;

    const float MIN_DELTA;
    const float MAX_DELTA;

    EngineCoreBase();

    virtual int init() = 0;
    static bool didInit;

    void initLight();
    void updateRatio();

    virtual ~EngineCoreBase(){};

    void run();

    virtual float getDeltaTime() = 0;
    virtual void checkEvents() = 0;
    virtual void swapBuffers() = 0;

    virtual void keyPressed(char k) = 0;
    virtual void update(const float delta) = 0;
    virtual void singleUpdate(const float delta) = 0;
    virtual void redraw() = 0;
    virtual void mouseMove(const int dx, const int dy) = 0;

    void updateWindowRatio();
    void performFrame(const float realUnscaledDelta);

    int width;
    int height;

private:
    bool goingToUpdateRatio;
};

#endif // ENGINECORE_H
