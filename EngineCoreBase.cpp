///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCoreBase.cpp

#include "EngineCoreBase.h"

EngineCoreBase::EngineCoreBase() :  MIN_TIME_SCALE(0.25),       MAX_TIME_SCALE(15.0),
                                    MIN_UPDATES_PER_FRAME(1),   MAX_UPDATES_PER_FRAME(1000),
                                    MIN_DELTA(0.007),           MAX_DELTA(0.15)
{
    timeScale = 2.0;
    updatesPerFrame = 10;
    goingToUpdateRatio = true;

    width = 1280;
    height = 720;
}

void EngineCoreBase::updateRatio()
{
    goingToUpdateRatio = true;
}

void EngineCoreBase::run()
{
    while (true)
    {
        checkEvents();
        performFrame(getDeltaTime());
    }
}

void EngineCoreBase::performFrame(const float realUnscaledDelta)
{
    float delta = realUnscaledDelta * timeScale;
    if (delta > MAX_DELTA) delta = MAX_DELTA;
    if (delta < MIN_DELTA) delta = MIN_DELTA;

    singleUpdate(delta);

    for(int i=0; i < updatesPerFrame; i++)
    {
        update(delta);
    }

    drawFrame();
}

void EngineCoreBase::updateWindowRatio()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float screenRatio = (float)width / height;
    glViewport(0, 0, width, height);
    glFrustum(-1.0 * screenRatio, 1.0 * screenRatio, -1.0, 1.0, 5.0, 1000.0);
}

void EngineCoreBase::drawFrame()
{
    if (goingToUpdateRatio)
    {
        updateWindowRatio();
        goingToUpdateRatio = false;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.0f, 0.0f, 5.0f);

    redraw();

    swapBuffers();
    glFlush();
}

void EngineCoreBase::initLight()
{
    const GLfloat lambient[]    = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat ldiffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat lspecular[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat lposition[]   = { 30.0f, 30.0f, 30.0f, 0.0f };

    const GLfloat mambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mdiffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mspecular[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat shininess[]   = { 100.0f };

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
