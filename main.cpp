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
    EngineCore::SetCmdArgs(argc, argv);

    cout<<"      Projekt PROI 18L"<<endl;
    cout<<"      EN: Simulator of city traffic"<<endl;
    cout<<"      PL: Symulator ruchu ulicznego"<<endl;
    cout<<"      by Robert Dudzinski"<<endl;
    cout<<endl<<endl;
    cout<<"   Steering: "<<endl<<endl;
    cout<<" W,A,S,D - movement"<<endl;
    cout<<" Q, E    - vertical movement"<<endl;
    cout<<" dragging mouse cursor - rotating camera"<<endl;
    cout<<" ESC - exit"<<endl;

    try
    {
        Simulator *simulator = &Simulator::getInstance();

        simulator->loadRoad("exampleRoad.txt");
        simulator->loadPriority("examplePriority.txt");
        simulator->run();
    }
    catch (string e)
    {
        cout << "ERROR: " << e;
    }

    return 0;
}
