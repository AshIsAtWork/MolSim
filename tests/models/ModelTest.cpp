//
// Created by daniel on 17.06.24.
//

#include <gtest/gtest.h>

#include "../../src/models/linkedCells/LinkedCells.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"

/**
 * If gravity is toggled off, no gravity should be applied.
 * --> We use here the linked cells model for testing, but it works with all models.
 */
TEST(ModelTest, applyGravityOff) {
    LeonardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective,
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective
    };

    LinkedCells linkedCellModel = {lJF, 0.00005, {3, 3, 3}, 3, FileHandler::outputFormat::vtk, boundaries, false};
    Particle p{{4.5,4.5,0},{0,0,0},1};
    linkedCellModel.addParticle(p);
    ASSERT_EQ(linkedCellModel.getParticles().size(), 1);
    linkedCellModel.step(1);
    linkedCellModel.getParticles().applyToEachParticleInDomain([](Particle& p) {
        std::array<double, 3> expected = {0,0,0};
       ASSERT_TRUE(p.getF() == expected);
    });
}

/**
 * If gravity is toggled on, gravity should be applied according to m*g along the y-axis.
 * --> We use here the linked cells model for testing, but it works with all models.
 */
TEST(ModelTest, applyGravityOn) {
    LeonardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective,
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective
    };

    LinkedCells linkedCellModel = {lJF, 0.00005, {9, 9, 0}, 3, FileHandler::outputFormat::vtk, boundaries, true, 10};
    Particle p{{4.5,4.5,0},{0,0,0},1};
    linkedCellModel.addParticle(p);
    ASSERT_EQ(linkedCellModel.getParticles().size(), 1);
    linkedCellModel.step(1);
    linkedCellModel.getParticles().applyToEachParticleInDomain([](Particle& p) {
        std::array<double, 3> expected = {0,10,0};
       ASSERT_TRUE(p.getF() == expected);
    });
}
