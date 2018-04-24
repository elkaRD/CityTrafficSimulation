#include "Simulator.h"
using namespace std;

int main(int argc, char** argv)
{
    Simulator::init(argc, argv);
    Simulator simulator;
    simulator.loadRoad("example.txt");
    simulator.run();

    return 0;
}
