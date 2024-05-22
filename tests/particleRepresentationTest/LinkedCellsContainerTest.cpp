//
// Created by daniel on 21.05.24.
//

#include <algorithm>
#include <gtest/gtest.h>
#include "particleRepresentation/LinkedCellsContainer.h"
#include <spdlog/spdlog.h>

#include "fileHandling/FileHandler.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"


/**
 * Does threeDtoOneD calculate the correct index?
 * Results for reference are calculated by hand.
 */

TEST(ThreeDtoOneD, BasicTest) {
    LinkedCellsContainer lcc{{3, 3, 3}, 1};
    EXPECT_EQ(lcc.threeDToOneD(0,0,0), 0);
    EXPECT_EQ(lcc.threeDToOneD(4,4,4), 124);
    EXPECT_EQ(lcc.threeDToOneD(1,2,3), 86);
}

/**
 * Does threeDtoOneD calculate the correct index if we have only 2D data?
 * Results for reference are calculated by hand.
 */

TEST(ThreeDtoOneD, TwoDTest) {
    LinkedCellsContainer lcc{{3, 3, 0}, 1};
    EXPECT_EQ(lcc.threeDToOneD(0,0,0), 0);
    EXPECT_EQ(lcc.threeDToOneD(4,4,0), 24);
    EXPECT_EQ(lcc.threeDToOneD(2,2,0), 12);
}

/**
 * Does oneDtoThreeD calculate the correct coordinates?
 * Results for reference are calculated by hand.
 */

TEST(OneDToThreeD, BasicTest) {
    LinkedCellsContainer lcc{{1, 2, 3}, 1};
    std::array<int, 3> res = {0, 0, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(0) == res);
    res = {1, 1, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(4) == res);
    std::cout << lcc.oneDToThreeD(44);
    res = {2, 2, 3};
    EXPECT_TRUE(lcc.oneDToThreeD(44) == res);
    res = {2, 3, 4};
    EXPECT_TRUE(lcc.oneDToThreeD(59) == res);
}

/**
 * Does oneDtoThreeD calculate the correct coordinates?
 * Results for reference are calculated by hand.
 */

TEST(OneDToThreeD, TwoDTest) {
    LinkedCellsContainer lcc{{1, 2, 0}, 1.5};
    std::array<int, 3> res = {0, 0, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(0) == res);
    res = {2, 2, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(8) == res);
    res = {2, 1, 0};
    EXPECT_TRUE(lcc.oneDToThreeD(5) == res);
}

/**
 * Are particles assigned to their correct cells by their position?
 * Results for reference are calculated by hand.
 */

TEST(CalcCellIndex, BasicTest) {
    LinkedCellsContainer lcc{{3, 3, 3}, 1};
    //Particles
    std::array<double, 3> p1 = {0, 0, 0};
    std::array<double, 3> p2 = {1.75, 0.66, 1.234};
    std::array<double, 3> p3 = {2.2, 1.2, 0};
    std::array<double, 3> p4 = {1.7, 3.1, 0};
    std::array<double, 3> p5 = {3, 3, 3};
    std::array<double, 3> p6 = {-10.3, 0, -1};

    EXPECT_EQ(lcc.calcCellIndex(p1), 31);
    EXPECT_EQ(lcc.calcCellIndex(p2), 57);
    EXPECT_EQ(lcc.calcCellIndex(p3), 38);
    EXPECT_EQ(lcc.calcCellIndex(p4), 47);
    EXPECT_EQ(lcc.calcCellIndex(p5), 124);
    EXPECT_EQ(lcc.calcCellIndex(p6), 5);
}

/**
 * Are particles assigned to their correct cells by their position in 2D?
 * Results for reference are calculated by hand.
 */

TEST(CalcCellIndex, Test2D) {
    LinkedCellsContainer lcc{{3, 3, 0}, 0.9};
    //Particles
    std::array<double, 3> p1 = {0, 0, 0};
    std::array<double, 3> p2 = {1.75, 1.66, 0};
    std::array<double, 3> p3 = {3.1, 3.1, 0};

    /**
    * Are particles assigned to their correct cells by their position?
    */

    EXPECT_EQ(lcc.calcCellIndex(p1), 6);
    EXPECT_EQ(lcc.calcCellIndex(p2), 12);
    EXPECT_EQ(lcc.calcCellIndex(p3), 24);
}

/**
 * Test, if all halo cell indizes are calculated correctly
 * Results for reference are calculated by hand.
 */

TEST(HaloCellIndizes, BasicTest) {
    LinkedCellsContainer lcc{{1, 2, 1}, 1};
    //If not all cells are there, it does not make sense to continue this test
    ASSERT_EQ(lcc.getHaloCells().size(), 34);
    for (int i = 0; i < 36; i++) {
        if (i != 16 && i != 19) {
            bool isPresent = false;
            for (int j = 0; j < 34; j++) {
                if(lcc.getHaloCells()[j] == i) {
                    isPresent = true;
                }
            }
            EXPECT_TRUE(isPresent);
        }
    }
}

TEST(IterationScheme, BasicTest) {
    LinkedCellsContainer lcc{{180, 90, 0}, 3};
    double current_time = 0;
    double endT = 20;
    double deltaT = 0.0005;
    int iteration = 0;

    ParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {20,20,0}, 100, 20 ,1 , 1.1225, 1 ,{0,0,0});
    ParticleGenerator::generateCuboid(pc, {70,60,0}, 20, 20 ,1 , 1.1225, 1 ,{0,-10,0});

    //Calculate the initial forces before starting the simulation
    LeonardJonesForce force;
    for(Particle p : pc) {
        lcc.add(p);
    }
    lcc.plot(iteration);
    lcc.updateForces(force);

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endT) {
        // calculate new x
        lcc.updatePositions(deltaT);
        // calculate new f
        lcc.updateForces(force);
        // calculate new v
        lcc.updateVelocities(deltaT);
        // update cells of particles
        lcc.updateCells();
        iteration++;
        if(iteration % 10 == 0) {
            lcc.plot(iteration);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}




