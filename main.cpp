#include "Simulator.h"
using namespace std;

int main(int argc, char** argv)
{
    Simulator::init(argc, argv);
    Simulator simulator;
    simulator.loadRoad("e3.txt");
    simulator.loadPriority("p1.txt");
    simulator.run();

    return 0;
}
