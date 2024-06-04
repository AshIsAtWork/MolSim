//
// Created by cle on 6/2/24.
//

#include <gtest/gtest.h>
#include "fileHandling/reader/XMLReader/XMLReader.h"

class XMLReaderTest : public ::testing::Test {
protected:
    enumsStructs::SimulationSettings simulationSettings;
    const std::string basePath = "../tests/testData/XMLTestFiles/";

    int readXMLFile(const std::string &filename) {
        std::string filePath = basePath + filename;
        return XMLReader::readFile(filePath, simulationSettings);
    }
};

/**
 * Simple test that checks whether all the parameters in the test file are being parsed correctly.
 * Test file is the 2D-cuboid-collision.xml form assignment 3
 */

TEST_F(XMLReaderTest, Basic_XMLTest) {
    ASSERT_EQ(readXMLFile("XMLTest1.xml"), 0);

    // General
    EXPECT_EQ(simulationSettings.outputFileName, "Collision");
    EXPECT_EQ(simulationSettings.outputFrequency, 100);

    // Model
    EXPECT_EQ(simulationSettings.model, enumsStructs::TypeOfModel::linkedCells);

    // Parameters for LinkedCellModel
    EXPECT_EQ(simulationSettings.parametersLinkedCells.deltaT, 0.00005);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.endT, 10);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.epsilon, 5);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.sigma, 1);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.force, enumsStructs::TypeOfForce::leonardJonesForce);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.rCutOff, 3);

    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[0], 180);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[1], 90);
    EXPECT_EQ(simulationSettings.parametersLinkedCells.domainSize[2], 0);

    std::vector<std::pair<enumsStructs::Side, enumsStructs::BoundaryCondition>> expectedBoundarySettings = {
            {enumsStructs::Side::front, enumsStructs::BoundaryCondition::reflective},
            {enumsStructs::Side::right, enumsStructs::BoundaryCondition::reflective},
            {enumsStructs::Side::back, enumsStructs::BoundaryCondition::reflective},
            {enumsStructs::Side::left, enumsStructs::BoundaryCondition::reflective},
            {enumsStructs::Side::top, enumsStructs::BoundaryCondition::reflective},
            {enumsStructs::Side::bottom, enumsStructs::BoundaryCondition::reflective}
    };

    for (size_t i = 0; i < expectedBoundarySettings.size(); ++i) {
        EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[i].first, expectedBoundarySettings[i].first);
        EXPECT_EQ(simulationSettings.parametersLinkedCells.boundarySettings[i].second, expectedBoundarySettings[i].second);
    }

    // Cuboid 1
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

    // Cuboid 2
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
}

/**
 * The following files were constructed to model them the resprective edge cases:
 *
 *      XMLTest3.xml: outputFileName is digit
 *      XMLTest4.xml: outputFrequency is string
 *      XMLTest5.xml: required element is missing
 *      XMLTest6.xml: file is empty
 *      XMLTest7.xml: file name is invalid
 *      XMLTest8.xml: negative output frequency
 *      XMLTest9.xml: invalid model name
 *      XMLTest10.xml: negative t_end
 *      XMLTest11.xml: negative delta_t
 *      XMLTest12.xml: invalid force
 *      XMLTest13.xml: negative sigma
 *      XMLTest14.xml: negative epsilon
 *      XMLTest15.xml: DomainSize elements
 *      XMLTest16.xml: negative Cuboids size
 *      XMLTest17.xml: negative or zero N1/N2/N3 in Cuboids
 *      XMLTest18.xml: negative or zero Distance in Cuboids
 *      XMLTest19.xml: invalid DimensionBrownian in Cuboids
 *      XMLTest20.xml: invalid mass in Cuboids
 *      XMLTest21.xml: invalid brownian in Cuboids
 */

class XMLReaderFailureTest : public XMLReaderTest, public ::testing::WithParamInterface<std::string> {};

TEST_P(XMLReaderFailureTest, FailureCases) {
    ASSERT_EQ(readXMLFile(GetParam()), 1);
}

INSTANTIATE_TEST_SUITE_P(FailureTests, XMLReaderFailureTest, ::testing::Values(
        "XMLTest3.xml",
//        "XMLTest4.xml",
        "XMLTest5.xml",
        "XMLTest6.xml",
        "XMLTest7.xml",
        "XMLTest8.xml",
        "XMLTest9.xml",
        "XMLTest10.xml",
        "XMLTest11.xml",
        "XMLTest12.xml",
        "XMLTest13.xml",
        "XMLTest14.xml",
        "XMLTest15.xml",
        "XMLTest16.xml",
        "XMLTest17.xml",
        "XMLTest18.xml",
        "XMLTest19.xml",
        "XMLTest20.xml",
        "XMLTest21.xml"
));
