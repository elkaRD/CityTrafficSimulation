///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: main.cpp


#include "Simulator.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    try
    {
        Simulator *simulator = Simulator::getInstance(argc, argv);

        simulator->loadRoad("e4.txt");
        simulator->loadPriority("p2.txt");
        simulator->run();
    }
    catch (string e)
    {
        cout << "ERROR: " << e;
    }

    return 0;
}
