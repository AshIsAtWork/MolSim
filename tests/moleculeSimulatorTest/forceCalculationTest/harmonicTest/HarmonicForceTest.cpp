//
// Created by daniel on 30.06.24.
//

#include <gtest/gtest.h>

#include "fileHandling/reader/XMLHandling/ConfigurationFile.h"
#include "moleculeSimulator/Simulator.h"
#include "particleRepresentation/particle/Particle.h"


/**
 * Test, if harmonic force is zero when current distance and average distance are equal
 */

TEST(HarmonicForceTest, ZeroForces) {
    Particle p1{{5, 0, 5}, {0, 0, 0}, 1, 1, 1, 1};
    Particle p2{{7, 0, 5}, {0, 0, 0}, 1, 1, 1, 1};
    HarmonicForce force{1,2};
    std::array<double, 3> expected = {0,0,0};
    EXPECT_TRUE(force.compute(p1, p2) == expected);
}
