#include "Simulator.h"
using namespace std;

int main(int argc, char** argv)
{
    Simulator::init(argc, argv);
    Simulator *simulator = Simulator::getInstance();

    simulator->loadRoad("e4.txt");
    simulator->loadPriority("p2.txt");
    simulator->run();

    return 0;
}
