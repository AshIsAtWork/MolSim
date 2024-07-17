//
// Created by daniel on 13.07.24.
//

#include <gtest/gtest.h>

#include "fileHandling/reader/XMLReader/XMLReader.h"
#include "moleculeSimulator/Simulator.h"

/**
 * Compute once the results sequentially. These values are taken as reference for the three parallel versions.
 */
class ParallelizationStrategiesTest: public ::testing::Test {
protected:
    std::unique_ptr<Simulator> sequentialSimulation;
    std::vector<Particle> expected;

    ParallelizationStrategiesTest()
    {
        SimulationSettings simulationSettings;
        std::string filepath = "../tests/testData/ParallelizationStrategiesTestData.xml";
        XMLReader::readFile(filepath,simulationSettings);
        sequentialSimulation = std::make_unique<Simulator>(simulationSettings,FileHandler::outputFormat::vtk);
        sequentialSimulation->run(true);
        sequentialSimulation->getParticles().applyToEachParticle([this](Particle& p)
        {
            expected.push_back(p);
        });
    }

    static bool compareValues(std::array<double, 3> v1, std::array<double,3> v2)
    {
        return abs(v1[0] - v2[0]) <= 1e-9
        and abs(v1[1] - v2[1]) <= 1e-9
        and abs(v1[1] - v2[1]) <= 1e-9;
    }

    static bool compareResults(std::vector<Particle>& particles1, std::vector<Particle>& particles2)
    {
        if(particles1.size() != particles2.size())
        {
            return false;
        }
        for(size_t i = 0; i < particles1.size(); i++)
        {
            //Check, if values are equal.
            if(!(compareValues(particles1[i].getOldF(),particles2[i].getOldF())
            and compareValues(particles1[i].getF(),particles2[i].getF())
            and compareValues(particles1[i].getX(),particles2[i].getX())
            and compareValues(particles1[i].getV(),particles2[i].getV())))
            {
                return false;
            }
        }
        return true;
    }
};

/**
 * Compare results of parallelization strategie linear against results of sequential program
 */

TEST_F(ParallelizationStrategiesTest, ParallelizationStrategy_Linear)
{
    SimulationSettings simulationSettings;
    std::string filepath = "../tests/testData/ParallelizationStrategiesLinear.xml";
    XMLReader::readFile(filepath,simulationSettings);
    std::vector<Particle> result;
    sequentialSimulation = std::make_unique<Simulator>(simulationSettings,FileHandler::outputFormat::vtk);
    sequentialSimulation->run(true);
    sequentialSimulation->getParticles().applyToEachParticle([&result](Particle& p)
        {
           result.push_back(p);
        });
    EXPECT_TRUE(compareResults(expected,result));
}

/**
 * Compare results of parallelization strategie skipping against results of sequential program
 */

TEST_F(ParallelizationStrategiesTest, ParallelizationStrategy_Skipping)
{
    SimulationSettings simulationSettings;
    std::string filepath = "../tests/testData/ParallelizationStrategiesSkipping.xml";
    XMLReader::readFile(filepath,simulationSettings);
    std::vector<Particle> result;
    sequentialSimulation = std::make_unique<Simulator>(simulationSettings,FileHandler::outputFormat::vtk);
    sequentialSimulation->run(true);
    sequentialSimulation->getParticles().applyToEachParticle([&result](Particle& p)
        {
           result.push_back(p);
        });
    EXPECT_TRUE(compareResults(expected,result));
}

/**
 * Compare results of parallelization strategie reduction against results of sequential program
 */

TEST_F(ParallelizationStrategiesTest, ParallelizationStrategy_Reduction)
{
    SimulationSettings simulationSettings;
    std::string filepath = "../tests/testData/ParallelizationStrategiesSkipping.xml";
    XMLReader::readFile(filepath,simulationSettings);
    std::vector<Particle> result;
    sequentialSimulation = std::make_unique<Simulator>(simulationSettings,FileHandler::outputFormat::vtk);
    sequentialSimulation->run(true);
    sequentialSimulation->getParticles().applyToEachParticle([&result](Particle& p)
        {
           result.push_back(p);
        });
    EXPECT_TRUE(compareResults(expected,result));
}
