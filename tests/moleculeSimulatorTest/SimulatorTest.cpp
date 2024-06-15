////
//// Created by daniel on 11.05.24.
////
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "moleculeSimulator/Simulator.h"
#include "moleculeSimulator/forceCalculation/gravity/Gravity.h"

/**
 * Test some edge cases...
 */

/**
 * The specified file is empty. In this case the program should terminate unsuccessfully.
 */

TEST(SimulatorTest, EmptyFile) {
    std::string filename = "name";
    DirectSumSimulationParameters dS = {0.0001, 0.001, TypeOfForce::gravity};

    std::string filepath = "../tests/testData/Empty.txt";
    EXPECT_DEATH(Simulator simulator(dS,filepath, FileHandler::outputFormat::vtk,10,filename), "");
}

/**
 * The specified file does not exist. In this case the program should terminate unsuccessfully.
 */

TEST(SimulatorTest, FileDoesNotExist) {
    std::string filename = "name";
    DirectSumSimulationParameters dS = {0.0001, 0.001, TypeOfForce::gravity};
    std::string filepath = "IDoNotExist";
    EXPECT_DEATH(Simulator simulator(dS,filepath, FileHandler::outputFormat::vtk,10,filename), "");
}
