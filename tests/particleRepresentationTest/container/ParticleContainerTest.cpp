//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>
#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"
#include <spdlog/spdlog.h>


/**
 * Set up test environment for each test.
 */
class ParticleContainerTest : public testing::Test {
protected:
    DefaultParticleContainer pc;
    Particle p;

    ParticleContainerTest() : pc{10} {}

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }
};

/**
 * Does the add function acutally add the particle p to the container?
 */

TEST_F(ParticleContainerTest, AddTest) {
    pc.add(p);
    EXPECT_EQ(pc.size(), 1);
    EXPECT_TRUE(pc.at(0) == p);
}

/**
 * Is out of range access recognised and can the added particle be accessed?
 */

TEST_F(ParticleContainerTest, AccesingContainerWithAtMethod) {
    pc.add(p);
    EXPECT_TRUE(pc.at(0) == p);
    EXPECT_THROW(pc.at(1), std::out_of_range);
}

/**
 * Is the size of the container correct?
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
 *Is the capacity set correctly?
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
 *Does the reverse method reserve space as specified?
 */

TEST_F(ParticleContainerTest, ReserveTest) {
    pc.reserve(20);
    EXPECT_EQ(pc.capacity(), 20);
    pc.reserve(100);
    EXPECT_EQ(pc.capacity(), 100);
}
