///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Colors.cpp


#include "Colors.h"

Vec3 Colors::getRandomColor()
{
    static Colors *instance;
    if (instance == nullptr)
    {
        instance = new Colors();
    }
    return instance->pickRandom();
}

Colors::Colors()
{
    amount = 6;
    colors = new Vec3[amount];

    colors[0] = Vec3(1, 0, 0);
    colors[1] = Vec3(0, 1, 0);
    colors[2] = Vec3(0, 0, 1);
    colors[3] = Vec3(1, 0, 1);
    colors[4] = Vec3(1, 1, 0);
    colors[5] = Vec3(0, 1, 1);
}

Vec3 Colors::pickRandom()
{
    return colors[rand() % amount];
}
