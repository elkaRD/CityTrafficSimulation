///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: Colors.h


#ifndef COLORS_H
#define COLORS_h

#include <cstdlib>
#include "Vec3.h"

class Colors
{
public:
    static Vec3 getRandomColor();

private:
    Vec3 *colors;
    int amount;
    Colors();

    Vec3 pickRandom();
};

#endif // COLORS_H
