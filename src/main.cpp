///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: main.cpp


#include "simulator/Simulator.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    EngineCore::SetCmdArgs(argc, argv);

    cout << "      Project for OOP subject at Warsaw University of Technology" << endl;
    cout << "      City traffic simulation" << endl;
    cout << "      Copyright (C) Robert Dudzinski 2018" << endl;
    cout << endl << endl;
    cout << "   Steering: " << endl << endl;
    cout << " W,A,S,D - movement" << endl;
    cout << " Q, E    - vertical movement" << endl;
    cout << " dragging mouse cursor - rotating camera" << endl;
    cout << " ESC - exit" << endl;

    try
    {
        Simulator *simulator = &Simulator::getInstance();

        simulator->loadRoad("exampleRoad.txt");
        simulator->loadPriority("exampleRightOfWay.txt");
        simulator->run();
    }
    catch (string e)
    {
        cout << "ERROR: " << e;
    }

    return 0;
}
