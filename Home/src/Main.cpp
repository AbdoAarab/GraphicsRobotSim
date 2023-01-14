#include "RobotSim.h"

int main()
{
    RobotSim simulation; // Initialize the Simulation
    simulation.setDebug(ALL_DEBUG);          // Set debug level
    simulation.run();                        // Run the simulation
    return 0;
}