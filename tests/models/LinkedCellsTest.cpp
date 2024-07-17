//
// Created by daniel on 28.05.24.
//

#include <gtest/gtest.h>

#include "../../src/models/linkedCells/LinkedCells.h"
#include "moleculeSimulator/forceCalculation/lennardJones/LennardJonesForce.h"

using namespace enumsStructs;

bool isInDomain(Particle &p) {
    std::array<double, 3> lowerBound = {0, 0, 0};
    std::array<double, 3> upperBound = {3, 3, 3};
    return lowerBound <= p.getX() && p.getX() < upperBound;
}

bool noExplosion(Particle &p) {
    //The upper bound is set here arbitrarily to 1000. Is it not so clear what to choose here. For sure, the velocity of the particle should
    //not get to high which is an indicator for an explosion.
    return abs(p.getV()[0]) <= 10e3 && abs(p.getV()[1]) <= 10e3 && abs(p.getV()[2]) <= 10e3;
}

bool isParticleOnTrack(Particle &p) {
    return p.getX()[1] == 4.5
           and p.getX()[2] == 0
           and 0 <= p.getX()[0] and p.getX()[0] < 9;
}

/**
 * Here we test, if the implementation of the reflective boundaries does work.
 * Testing this thoroughly is difficult, because there are only qualitative requirements.
 * The key requirements are:
 *                          1.) No particle should leave the domain
 *                          2.) There should be no explosions
 *
 * To test this, we put one particle into a small domain, let it fly around and let the simulation run for some time.
 * After each step we check, if the particle is still within the domain and that velocities and forces do not exceed
 * some threshold
 *
 * --> One have to keep in mind to choose deltaT small enough to ensue that the method is stable. Otherwise, it could happen
 *     that the particle escapes the simulation domain, because the force applied by the virtual ghost particle has not been yet
 *     big enough to divert the particle from the border.
 */

TEST(LinkedCellsTest, ReflectiveBoundries) {
    LennardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective,
        BoundaryCondition::reflective, BoundaryCondition::reflective, BoundaryCondition::reflective
    };

    LinkedCells linkedCellModel = {lJF, 0.00005, {3, 3, 3}, 3, FileHandler::outputFormat::vtk, boundaries, false};
    double current_time = 0;

    int iteration = 0;

    linkedCellModel.updateForces();

    //We use a velocity vector here that will guarantee that all boundaries are being hit.

    Particle p = {{1.5, 1.5, 1.5}, {10, 20, 30}, 1};
    linkedCellModel.addParticle(p);

    while (current_time < 5) {
        linkedCellModel.step(1);
        iteration++;
        current_time += 0.00005;
        //Check conditions
        linkedCellModel.getParticles().applyToEachParticle([](Particle &p) {
            //We want to stop the simualtion immediately, if the particle is outside the domain or an explosion was detected
            ASSERT_TRUE(isInDomain(p));
            ASSERT_TRUE(noExplosion(p));
        });
    }
}

/**
 * Here we test, if the boundary condition outflow works properly for each side of the simulation domain. To test this, we send
 * one particle towards each boundary and make sure that it will cross it. After the particle crossed the boundary it should be
 * deleted.
 */

TEST(LinkedCellsTest, Outflow) {
    LennardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::outflow, BoundaryCondition::outflow, BoundaryCondition::outflow,
        BoundaryCondition::outflow, BoundaryCondition::outflow, BoundaryCondition::outflow
    };

    LinkedCells linkedCellModel = {
        lJF, 0.05, {3, 3, 3}, 3, FileHandler::outputFormat::vtk, boundaries, false
    };
    double current_time = 0;

    int iteration = 0;

    //Send one particle in each direction

    Particle pUp = {{1.5, 1.5, 1.6}, {0, 0, 10}, 1};
    Particle pDown = {{1.5, 1.5, 1.4}, {0, 0, -10}, 1};
    Particle pRight = {{1.6, 1.5, 1.5}, {10, 0, 0}, 1};
    Particle pLeft = {{1.4, 1.5, 1.5}, {-10, 0, 0}, 1};
    Particle pForward = {{1.5, 1.6, 1.5}, {0, 10, 0}, 1};
    Particle pBackward = {{1.5, 1.4, 1.5}, {0, -10, 0}, 1};

    linkedCellModel.addParticle(pUp);
    linkedCellModel.addParticle(pDown);
    linkedCellModel.addParticle(pRight);
    linkedCellModel.addParticle(pLeft);
    linkedCellModel.addParticle(pForward);
    linkedCellModel.addParticle(pBackward);

    ASSERT_EQ(linkedCellModel.getParticles().size(), 6);

    linkedCellModel.updateForces();

    while (current_time < 5) {
        linkedCellModel.step(1);
        iteration++;
        current_time += 0.05;
    }

    //All particles should be deleted after crossing one border
    EXPECT_EQ(linkedCellModel.getParticles().size(), 0);
}

/**
 * Here we test, if the boundary condition periodic works correctly. The implementation of this boundary condition depends on a
 * lot of different methods and helper methods. Testing each of them individually as it should be done in the best case
 * would take a lot of time that we unfortunately do not have. Therefore, we try to test the boundary condition as a whole.
 * To further simplify the testing, we are considering here the 2D case, because in every task, we are only using the 2D setting and
 * the 3D setting is much more complex and therefore really hard to test.
 */

/**
 *  Test 1: Here we check, if a particle which has left the domain one the right side appears again on the opposite side. We are checking it here
 *          for one side only, because it is similar for each side.
 */


TEST(LinkedCellsTest, Periodic_PostionUpdates) {
    LennardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::periodic, BoundaryCondition::periodic, BoundaryCondition::periodic,
        BoundaryCondition::periodic, BoundaryCondition::periodic, BoundaryCondition::periodic
    };

    LinkedCells linkedCellModel = {
        lJF, 0.05, {9, 9, 0}, 3, FileHandler::outputFormat::vtk, boundaries, false
    };
    double current_time = 0;

    int iteration = 0;

    //Send one particle in one direction and check, if it stays in the domain, is not deleted and always moving.

    Particle p = {{4.5, 4.5, 0}, {10, 0, 0}, 1};

    linkedCellModel.addParticle(p);

    ASSERT_EQ(linkedCellModel.getParticles().size(), 1);

    linkedCellModel.updateForces();

    while (current_time < 5) {
        double xCoordinateBefore;
        linkedCellModel.getParticles().applyToEachParticleInDomain([&xCoordinateBefore](Particle &p) {
            xCoordinateBefore = p.getX()[0];
        });
        linkedCellModel.step(1);
        double xCoordinateAfter;
        linkedCellModel.getParticles().applyToEachParticleInDomain([&xCoordinateAfter](Particle &p) {
            xCoordinateAfter = p.getX()[0];
        });
        //Particle should move along the x-axis.
        ASSERT_TRUE(xCoordinateBefore != xCoordinateAfter);
        //Particle should always stay on track.
        ASSERT_TRUE(isParticleOnTrack(p));
        iteration++;
        current_time += 0.05;
    }
}

/**
 * Test 2: The idea of this test is to send two particles starting from the middle of the domain into opposite directions
 *         along the x-axis. What we expect to happen is that the right and left boundary appear to be reflecting because
 *         both particles are pushing each other away from the boundary. What is happending here is in fact nothing else than
 *         the ghost particle implemenation does as part of reflective boundaries.
 *         --> We choose deltaT here small enough to guarantee that the simulation is stable.
 *         A visualisation can be found in the directory assignment-4 of the submission directory. It is called
 *         Test-Periodic-Boundaries-Visualisation.
 */


TEST(LinkedCellsTest, Periodic_Force_Calculation) {
    FileHandler file_handler;
    LennardJonesForce lJF;
    BoundarySet boundaries = {
        BoundaryCondition::periodic, BoundaryCondition::periodic, BoundaryCondition::periodic,
        BoundaryCondition::periodic, BoundaryCondition::periodic, BoundaryCondition::periodic
    };

    LinkedCells linkedCellModel = {
        lJF, 0.0005, {9, 9, 0}, 3, FileHandler::outputFormat::vtk, boundaries, false
    };
    double current_time = 0;

    int iteration = 1;

    //Send one particle in one direction and check, if it stays in the domain, is not deleted and always moving.

    Particle pRight = {{5, 4.5, 0}, {10, 0, 0}, 1,1};
    Particle pLeft = {{4, 4.5, 0}, {-10, 0, 0}, 1,2};

    linkedCellModel.addParticle(pRight);
    linkedCellModel.addParticle(pLeft);

    ASSERT_EQ(linkedCellModel.getParticles().size(), 2);

    linkedCellModel.updateForces();
    std::string testName = "TestPeriodic";
    while (current_time < 10){
        linkedCellModel.getParticles().applyToEachParticle([](Particle& p) {
            //Y and Z coordinates should not change for both particles:
            ASSERT_TRUE(p.getX()[1] == 4.5);
            ASSERT_TRUE(p.getX()[2] == 0);

            if(p.getType() == 1) {
                //X coordinate of particle pRight should always stay between 4.5 and 9.
                ASSERT_TRUE(4.5 <= p.getX()[0] and p.getX()[0] <= 9);
            }else {
                //X coordinate of particle pLeft should always stay between 0 and 4.5.
                ASSERT_TRUE(0 <= p.getX()[0] and p.getX()[0] <= 4.5);
            }

        });
        linkedCellModel.step(1);
        if(iteration % 100 == 0) {
            file_handler.writeToFile(linkedCellModel.getParticles(),iteration,FileHandler::outputFormat::vtk,testName);
        }
        iteration++;
        current_time += 0.0005;
    }
}
