//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>
#include "moleculeSimulator/forceCalculation/gravity/Gravity.h"


/**
 *In this test we calculated the gravitational force between to particles by hand using the formula on worksheet1.
 *
 *To make our calculations traceable, we provide here the latex code of our formula:
 *
 *-\frac{24 \times 5}{16} \times \left( \left(\frac{1}{4}\right)^6
 *- 2 \times \left(\frac{1}{4}\right)^{12} \right) \times 4
 *
 *We calculate here only the value in the first dimension, because all other values must trivially be zero.
 */

TEST(GravityTest, CompareWithHandCalculatedResult) {
    Particle p1({4, 2, 1}, {0, 0, 0}, 1, 1);
    Particle p2({3, 5, 8}, {0, 0, 0}, 2, 1);
    Gravity g;

    auto force = g.compute(p1, p2);
    EXPECT_NEAR(force[0], -0.00441318342, 1e-9);
    EXPECT_NEAR(force[1], 0.01323955027, 1e-9);
    EXPECT_NEAR(force[2], 0.03089228396, 1e-9);
}

/**
 * Test, if the calculation adheres to Newton's third law of motion.
 */

TEST(GravityTest, NewtonsThirdLawOfMotion) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 1, 1);
    Particle p2({7, 3, 9}, {0, 0, 0}, 1, 1);
    Gravity g;

    auto force_p1p2 = g.compute(p1, p2);
    auto force_p2p1 = g.compute(p2, p1);
    EXPECT_NEAR(force_p1p2[0], -force_p2p1[0], 1e-9);
    EXPECT_NEAR(force_p1p2[1], -force_p2p1[1], 1e-9);
    EXPECT_NEAR(force_p1p2[2], -force_p2p1[2], 1e-9);
}

/**
 * Test some edge cases...
 */

/**
 * If the mass of one particle is 0, the gravitational force has to be 0.
 */

TEST(GravityTest, MassEqualsZero) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 0, 1);
    Particle p2({7, 3, 9}, {0, 0, 0}, 1, 1);
    Gravity g;

    auto force_p1p2 = g.compute(p1, p2);
    EXPECT_NEAR(force_p1p2[0], 0, 1e-9);
    EXPECT_NEAR(force_p1p2[1], 0, 1e-9);
    EXPECT_NEAR(force_p1p2[2], 0, 1e-9);
}

/**
 * The gravitational force that exerts a particle on itself is undefined.
 */

TEST(GravityTest, ForceOnItself) {
    Particle p1({1, 3, 4}, {0, 0, 0}, 1, 1);
    Gravity g;

    auto force_p1p1 = g.compute(p1, p1);
    EXPECT_TRUE(std::isnan(force_p1p1[0]));
    EXPECT_TRUE(std::isnan(force_p1p1[1]));
    EXPECT_TRUE(std::isnan(force_p1p1[2]));
}

/**
 * The gravitational force of two particles that are in the same place is undefined.
 */

TEST(GravityTest, PositionsEqual) {
    Particle p1({1, 1, 1}, {0, 0, 0}, 1, 1);
    Particle p2({1, 1, 1}, {1, 2, 3}, 5, 1);
    Gravity g;

    auto force_p1p2 = g.compute(p1, p2);
    EXPECT_TRUE(std::isnan(force_p1p2[0]));
    EXPECT_TRUE(std::isnan(force_p1p2[1]));
    EXPECT_TRUE(std::isnan(force_p1p2[2]));
}
