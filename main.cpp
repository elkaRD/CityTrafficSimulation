///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: main.cpp


#include "Simulator.h"
using namespace std;

int main(int argc, char** argv)
{
    //Simulator::init(argc, argv);
    Simulator *simulator = Simulator::getInstance(argc, argv);

    simulator->loadRoad("e4.txt");
    simulator->loadPriority("p2.txt");
    simulator->run();

    return 0;
}
