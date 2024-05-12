//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>
#include "particleRepresentation/ParticleContainer.h"
#include <spdlog/spdlog.h>


/**
 * Set up test environment for each test.
 */
class ParticleContainerTest : public testing::Test {
protected:
    ParticleContainer pc;
    Particle p;

    ParticleContainerTest() : pc{10} {}

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }
};

/**
 * Test, if the add function acutally adds the particle p to the container.
 */

TEST_F(ParticleContainerTest, AddTest) {
    pc.add(p);
    EXPECT_EQ(pc.size(), 1);
    EXPECT_TRUE(pc.at(0) == p);
}

/**
 * Test, if out of range access is recognised and the added particle can be accessed.
 */

TEST_F(ParticleContainerTest, AtTest) {
    pc.add(p);
    EXPECT_TRUE(pc.at(0) == p);
    EXPECT_THROW(pc.at(1), std::out_of_range);
}

/**
 *Test, if the size function works correctly.
 */

TEST_F(ParticleContainerTest, SizeTest) {
    pc.add(p);
    EXPECT_EQ(pc.size(), 1);

    for (int i = 0; i < 9; i++) {
        Particle pNew;
        pc.add(pNew);
    }

    EXPECT_EQ(pc.size(), 10);
}

/**
 *Test, if the capacity function works as expected.
 */

TEST_F(ParticleContainerTest, CapacityTest) {
    EXPECT_EQ(pc.capacity(), 10);

    for (int i = 0; i < 11; i++) {
        Particle pNew;
        pc.add(pNew);
    }

    //If capacity is exceeded, it should grow.
    EXPECT_GT(pc.capacity(), 10);
}

/**
 *Test, if reserve function is correct.
 */

TEST_F(ParticleContainerTest, ReserveTest) {
    pc.reserve(20);
    EXPECT_EQ(pc.capacity(), 20);
    pc.reserve(100);
    EXPECT_EQ(pc.capacity(), 100);
}
