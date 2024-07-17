//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>

#include "moleculeSimulator/forceCalculation/lennardJones/LennardJonesForce.h"


/**
 *In this test we calculated the Lennard Jones force between to particles by hand using the formula on worksheet2. The actual implemenation
 *uses a simplified version of this formula to increase efficiency, so this test checks as well, if our simplifications are correct.
 *
 *To make our calculations traceable, we provide here the latex code of our formula:
 *
 *-\frac{24 \times 5}{16} \times \left( \left(\frac{1}{4}\right)^6
 *- 2 \times \left(\frac{1}{4}\right)^{12} \right) \times 4
 *
 *We calculate here only the value in the first dimension, because all other values must trivially be zero.
 */

TEST(LennardJonesForceTest, CompareWithHandCalculatedResult) {
    Particle p1({4, 0, 0}, {0, 0, 0}, 1, 1);
    Particle p2({0, 0, 0}, {0, 0, 0}, 1, 1);
    LennardJonesForce lJF;

    auto force = lJF.compute(p1, p2);
    EXPECT_NEAR(force[0], -0.007320642471313, 1e-9);
    EXPECT_NEAR(force[1], 0, 1e-9);
    EXPECT_NEAR(force[2], 0, 1e-9);
}

/**
 * Test, if the calculation adheres to Newton's third law of motion.
 */

TEST(LennardJonesForceTest, NewtonsThirdLawOfMotion) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 1, 1);
    Particle p2({7, 3, 9}, {0, 0, 0}, 1, 1);
    LennardJonesForce lJF;

    auto force_p1p2 = lJF.compute(p1, p2);
    auto force_p2p1 = lJF.compute(p2, p1);
    EXPECT_NEAR(force_p1p2[0], -force_p2p1[0], 1e-9);
    EXPECT_NEAR(force_p1p2[1], -force_p2p1[1], 1e-9);
    EXPECT_NEAR(force_p1p2[2], -force_p2p1[2], 1e-9);
}

/**
 * Test some edge cases
 */

/**
 * The Lennard Jones force is independent of the mass of the particles.
 */

TEST(LennardJonesForceTest, MassEqualsZero) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 1, 1);
    Particle p2({7, 3, 9}, {0, 0, 0}, 1, 1);
    Particle p3({1, 3, 4}, {0, 0, 0}, 2, 1);
    Particle p4({7, 3, 9}, {0, 0, 0}, 5, 1);
    LennardJonesForce g;

    auto force_p1p2 = g.compute(p1, p2);
    auto force_p3p4 = g.compute(p3, p4);
    EXPECT_NEAR(force_p1p2[0], force_p3p4[0], 1e-9);
    EXPECT_NEAR(force_p1p2[1], force_p3p4[1], 1e-9);
    EXPECT_NEAR(force_p1p2[2], force_p3p4[2], 1e-9);
}

/**
 * The Lennard Jones force that exerts a particle on itself is undefined.
 */

TEST(LennardJonesForceTest, ForceOnItself) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 1, 1);
    LennardJonesForce g;

    auto force_p1p1 = g.compute(p1, p1);
    EXPECT_TRUE(std::isnan(force_p1p1[0]));
    EXPECT_TRUE(std::isnan(force_p1p1[1]));
    EXPECT_TRUE(std::isnan(force_p1p1[2]));
}

/**
 * The Lennard Jones force of two particles that are in the same place is undefined.
 */

TEST(LennardJonesForceTest, PositionsEqual) {
    Particle p1({1, 1, 1}, {0, 0, 0}, 1, 1);
    Particle p2({1, 1, 1}, {1, 2, 3}, 5, 1);
    LennardJonesForce g;

    auto force_p1p2 = g.compute(p1, p2);
    EXPECT_TRUE(std::isnan(force_p1p2[0]));
    EXPECT_TRUE(std::isnan(force_p1p2[1]));
    EXPECT_TRUE(std::isnan(force_p1p2[2]));
}
