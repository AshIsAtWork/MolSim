//
// Created by cle on 6/2/24.
//

#include <gtest/gtest.h>
#include "fileHandling/reader/XMLReader/XMLReader.h"

/**
 * Simple test that checks whether all the parameters in the test file are being parsed correctly.
 */

TEST(XMLReaderTest, Basic_LinkedCells_Cuboid_XMLTest) {
    enumsStructs::SimulationSettings simulationSettings;
    std::string originalFile = "../../input/assignment-3/2D-cuboid-collision.xml";
    std::string filename = "../tests/testData/XMLTest1.xml";
    ASSERT_EQ(XMLReader::readFile(filename, simulationSettings), 0);

    //General
    EXPECT_EQ(simulationSettings.outputFileName, "Collision");
    EXPECT_EQ(simulationSettings.outputFrequency, 100);

    //Model
    EXPECT_EQ(simulationSettings.model, enumsStructs::TypeOfModel::linkedCells);

    //Parameters for DirectSumModel
    EXPECT_EQ(simulationSettings.parametersDirectSum.deltaT, 0.00005);
    EXPECT_EQ(simulationSettings.parametersDirectSum.endT, 10);
    EXPECT_EQ(simulationSettings.parametersDirectSum.epsilon, 5);
    EXPECT_EQ(simulationSettings.parametersDirectSum.sigma, 1);
    EXPECT_EQ(simulationSettings.parametersDirectSum.force, enumsStructs::TypeOfForce::leonardJonesForce);

    //Parameters for LinkedCellModel
    EXPECT_EQ(simulationSettings.parametersLinkedCells.deltaT, 0.00005);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.endT, 10);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.epsilon, 5);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.sigma, 1);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.force, enumsStructs::TypeOfForce::leonardJonesForce);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.rCutOff, 3);



    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[0], 180);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[1], 90);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[2], 0);

    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[0].first, enumsStructs::Side::front);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[0].second, enumsStructs::BoundaryCondition::reflective);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[1].first, enumsStructs::Side::right);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[1].second, enumsStructs::BoundaryCondition::reflective);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[2].first, enumsStructs::Side::back);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[2].second, enumsStructs::BoundaryCondition::reflective);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[3].first, enumsStructs::Side::left);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[3].second, enumsStructs::BoundaryCondition::reflective);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[4].first, enumsStructs::Side::top);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[4].second, enumsStructs::BoundaryCondition::reflective);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[5].first, enumsStructs::Side::bottom);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[5].second, enumsStructs::BoundaryCondition::reflective);

    //Particles and objects of particles
    //Cuboid 1
    EXPECT_EQ(simulationSettings.cuboids[0].position[0], 20.0);
    EXPECT_EQ(simulationSettings.cuboids[0].position[1], 20.0);
    EXPECT_EQ(simulationSettings.cuboids[0].position[2], 0.0);

    EXPECT_EQ(simulationSettings.cuboids[0].dimensions[0], 100);
    EXPECT_EQ(simulationSettings.cuboids[0].dimensions[1], 20);
    EXPECT_EQ(simulationSettings.cuboids[0].dimensions[2], 1);

    EXPECT_EQ(simulationSettings.cuboids[0].h, 1.1225);
    EXPECT_EQ(simulationSettings.cuboids[0].mass, 1.0);

    EXPECT_EQ(simulationSettings.cuboids[0].initVelocity[0], 0);
    EXPECT_EQ(simulationSettings.cuboids[0].initVelocity[1], 0);
    EXPECT_EQ(simulationSettings.cuboids[0].initVelocity[2], 0);

    EXPECT_EQ(simulationSettings.cuboids[0].dimensionsBrownianMotion, 2);
    EXPECT_EQ(simulationSettings.cuboids[0].brownianMotionAverageVelocity, 0.1);

    //Cuboid 2
    EXPECT_EQ(simulationSettings.cuboids[1].position[0], 70.0);
    EXPECT_EQ(simulationSettings.cuboids[1].position[1], 60.0);
    EXPECT_EQ(simulationSettings.cuboids[1].position[2], 0.0);

    EXPECT_EQ(simulationSettings.cuboids[1].dimensions[0], 20);
    EXPECT_EQ(simulationSettings.cuboids[1].dimensions[1], 20);
    EXPECT_EQ(simulationSettings.cuboids[1].dimensions[2], 1);

    EXPECT_EQ(simulationSettings.cuboids[1].h, 1.1225);
    EXPECT_EQ(simulationSettings.cuboids[1].mass, 1.0);

    EXPECT_EQ(simulationSettings.cuboids[1].initVelocity[0], 0);
    EXPECT_EQ(simulationSettings.cuboids[1].initVelocity[1], -10);
    EXPECT_EQ(simulationSettings.cuboids[1].initVelocity[2], 0);

    EXPECT_EQ(simulationSettings.cuboids[1].dimensionsBrownianMotion, 2);
    EXPECT_EQ(simulationSettings.cuboids[1].brownianMotionAverageVelocity, 0.1);

    //unused objects: discs, particles

}
