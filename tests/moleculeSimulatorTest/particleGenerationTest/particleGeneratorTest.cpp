//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>

#include "fileHandling/FileHandler.h"
#include "moleculeSimulator/particleGeneration/ParticleGenerator.h"


/**
 * Set up test environment.
 */
class ParticleGeneratorTest_Cuboid : public ::testing::Test {
protected:
    DefaultParticleContainer pc;

    ParticleGeneratorTest_Cuboid() : pc{8} {
    };

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);

        ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 2, 2, 2, 1, 1, {0, 0, 0}, 3);
    }
};

/*
 *Part one: Testing the generateCuboid() method:
 */

/**
 * Is the number of created particles correct? It should be the product N1*N2*N3
 */

TEST_F(ParticleGeneratorTest_Cuboid, CuboidContainerSizeTest) {
    EXPECT_EQ(pc.size(), 8);
}

/**
 *Is each particle at the right position?
 */

TEST_F(ParticleGeneratorTest_Cuboid, CuboidPositionsTest) {
    std::array<double, 3> p0 = {0, 0, 0};
    std::array<double, 3> p1 = {0, 0, 1};
    std::array<double, 3> p2 = {0, 1, 0};
    std::array<double, 3> p3 = {0, 1, 1};
    std::array<double, 3> p4 = {1, 0, 0};
    std::array<double, 3> p5 = {1, 0, 1};
    std::array<double, 3> p6 = {1, 1, 0};
    std::array<double, 3> p7 = {1, 1, 1};

    ASSERT_EQ(pc.size(), 8);

    EXPECT_EQ(pc.at(0).getX(), p0);
    EXPECT_EQ(pc.at(1).getX(), p1);
    EXPECT_EQ(pc.at(2).getX(), p2);
    EXPECT_EQ(pc.at(3).getX(), p3);
    EXPECT_EQ(pc.at(4).getX(), p4);
    EXPECT_EQ(pc.at(5).getX(), p5);
    EXPECT_EQ(pc.at(6).getX(), p6);
    EXPECT_EQ(pc.at(7).getX(), p7);
}

/**
 * Is the mass of each particle set correctly?
 */

TEST_F(ParticleGeneratorTest_Cuboid, CuboidMassTest) {
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(pc.at(i).getM(), 1);
    }
}

/**
 * If one dimension is equal to zero, no particles should be created at all.
 */

TEST(ParticleGeneratorTest_Cuboid_2, OneDimensionZero) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 2, 0, 2, 1, 1, {0, 0, 0}, 3);
    EXPECT_EQ(pc.size(), 0);
}

/**
 * Does the function work correctly, when all dimensions are set differently?
 */

TEST(ParticleGeneratorTest_Cuboid_2, DifferentDimensions) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 10, 26, 8, 1, 1, {0, 0, 0}, 3);
    EXPECT_EQ(pc.size(), 10*26*8);
}

/**
 * Does the function work correctly, when the particle container is not empty at the beginning?
 */

TEST(ParticleGeneratorTest_Cuboid_2, ParticleContainerNotEmpty) {
    DefaultParticleContainer pc;
    Particle p;
    pc.add(p);
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 10, 10, 10, 1, 1, {0, 0, 0}, 3);
    EXPECT_EQ(pc.size(), 1001);
}

/**
 * Is the starting position correctly applied?
 */

TEST(ParticleGeneratorTest_Cuboid_2, NonZeroStartingPostion) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {10, 0, 0}, 1, 1, 1, 1, 1, {0, 0, 0}, 3);
    std::array<double, 3> pos = {10, 0, 0};
    ASSERT_EQ(pc.size(), 1);
    EXPECT_EQ(pc.at(0).getX(), pos);
}

/**
 * Is it possible, to add more than one cuboid to the same particle container?
 */

TEST(ParticleGeneratorTest_Cuboid_2, AddMultipleCuboids) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {10, 0, 0}, 1, 1, 1, 1, 1, {0, 0, 0}, 3);
    ParticleGenerator::generateCuboid(pc, {20, 0, 0}, 1, 10, 3, 1, 1, {0, 0, 0}, 3);
    EXPECT_EQ(pc.size(), 31);
}

/*
 *Part two: Testing the generateDisc() method:
 */

/*
 *Are all particles inside the specified cirlce?
 */

TEST(ParticleGeneratorTest_Disc, AllParticlesInsideTheCircle) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateDisc(pc, {0,0,0}, {0,0,0}, 10, 1.5, 1);
    for(Particle& p : pc) {
        EXPECT_LE(ArrayUtils::L2Norm(p.getX()), 13.5);
    }
    FileHandler handler;
    handler.writeToFile(pc, 1,FileHandler::outputFormat::vtk);
}

/*
 * Edge case, disc consists only of one particle, the particle in the center.
 */

TEST(ParticleGeneratorTest_Disc, OnlyOneParticleInTheCenter) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateDisc(pc, {0,0,0}, {0,0,0}, 1, 0.2, 1);
    EXPECT_EQ(pc.size(),1);
}

/*
 * Edge case, disc contains 0 particles
 */

TEST(ParticleGeneratorTest_Disc, ZeroParticles) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateDisc(pc, {0,0,0}, {0,0,0}, 0, 0.2, 1);
    EXPECT_EQ(pc.size(),0);
}

/*
 * Check, if the particles are positioned in a mesh with width h.
 */

TEST(ParticleGeneratorTest_Disc, MeshWidthCorrect) {
    DefaultParticleContainer pc, pcRef;
    ParticleGenerator::generateDisc(pc, {0,0,0}, {0,0,0}, 6, 1, 1);
    ParticleGenerator::generateCuboid(pcRef,{-5,-5,0},11,11,1,1,1,{0,0,0}, 3);
    //Brownian Motion has to be removed for this test.
    for(Particle& p: pcRef) {
        p.setV({0,0,0});
        p.setType(pc.at(0).getType());
    }
    for(Particle p : pc) {
        EXPECT_TRUE(pcRef.contains(p));
    }
}



