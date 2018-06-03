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
    cout<<"      Projekt PROI 18L"<<endl;
    cout<<"      Symulator ruchu ulicznego"<<endl;
    cout<<"      Autor: Robert Dudzinski"<<endl;
    cout<<endl<<endl;
    cout<<"   Sterowanie: "<<endl<<endl;
    cout<<" W,A,S,D - poruszanie się przdd, lewo, tyl, prawo"<<endl;
    cout<<" Q, E    - poruszanie się dol, gora"<<endl;
    cout<<" przeciaganie myszy - obracanie kamera"<<endl;
    cout<<" ESC - wyjscie"<<endl;

    try
    {
        Simulator *simulator = Simulator::getInstance(argc, argv);

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
