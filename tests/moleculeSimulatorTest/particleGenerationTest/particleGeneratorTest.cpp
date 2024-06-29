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

        ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 2, 2, 2, 1, 1, {0, 0, 0}, 3, 0.1);
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
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 2, 0, 2, 1, 1, {0, 0, 0}, 3, 0.1);
    EXPECT_EQ(pc.size(), 0);
}

/**
 * Does the function work correctly, when all dimensions are set differently?
 */

TEST(ParticleGeneratorTest_Cuboid_2, DifferentDimensions) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 10, 26, 8, 1, 1, {0, 0, 0}, 3, 0.1);
    EXPECT_EQ(pc.size(), 10*26*8);
}

/**
 * Does the function work correctly, when the particle container is not empty at the beginning?
 */

TEST(ParticleGeneratorTest_Cuboid_2, ParticleContainerNotEmpty) {
    DefaultParticleContainer pc;
    Particle p;
    pc.add(p);
    ParticleGenerator::generateCuboid(pc, {0, 0, 0}, 10, 10, 10, 1, 1, {0, 0, 0}, 3, 0.1);
    EXPECT_EQ(pc.size(), 1001);
}

/**
 * Is the starting position correctly applied?
 */

TEST(ParticleGeneratorTest_Cuboid_2, NonZeroStartingPostion) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {10, 0, 0}, 1, 1, 1, 1, 1, {0, 0, 0}, 3, 0.1);
    std::array<double, 3> pos = {10, 0, 0};
    ASSERT_EQ(pc.size(), 1);
    EXPECT_EQ(pc.at(0).getX(), pos);
}

/**
 * Is it possible, to add more than one cuboid to the same particle container?
 */

TEST(ParticleGeneratorTest_Cuboid_2, AddMultipleCuboids) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateCuboid(pc, {10, 0, 0}, 1, 1, 1, 1, 1, {0, 0, 0}, 3, 0.1);
    ParticleGenerator::generateCuboid(pc, {20, 0, 0}, 1, 10, 3, 1, 1, {0, 0, 0}, 3, 0.1);
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
    ParticleGenerator::generateDisc(pc, {0, 0, 0}, {0, 0, 0}, 10, 1.5, 1, 3, 0.1);
    for (Particle &p: pc) {
        EXPECT_LE(ArrayUtils::L2Norm(p.getX()), 13.5);
    }
}

/*
 * Edge case, disc consists only of one particle, the particle in the center.
 */

TEST(ParticleGeneratorTest_Disc, OnlyOneParticleInTheCenter) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateDisc(pc, {0, 0, 0}, {0, 0, 0}, 1, 0.2, 1, 3, 0.1);
    EXPECT_EQ(pc.size(), 1);
}

/*
 * Edge case, disc contains 0 particles
 */

TEST(ParticleGeneratorTest_Disc, ZeroParticles) {
    DefaultParticleContainer pc;
    ParticleGenerator::generateDisc(pc, {0, 0, 0}, {0, 0, 0}, 0, 0.2, 1, 3, 0.1);
    EXPECT_EQ(pc.size(), 0);
}

/*
 * Check, if the particles are positioned in a mesh with width h.
 */

TEST(ParticleGeneratorTest_Disc, MeshWidthCorrect) {
    DefaultParticleContainer pc, pcRef;
    ParticleGenerator::generateDisc(pc, {0, 0, 0}, {0, 0, 0}, 6, 1, 1, 3, 0.1);
    ParticleGenerator::generateCuboid(pcRef, {-5, -5, 0}, 11, 11, 1, 1, 1, {0, 0, 0}, 3, 0.1);
    //Brownian Motion has to be removed for this test.
    for (Particle &p: pcRef) {
        p.setV({0, 0, 0});
        p.setType(pc.at(0).getType());
    }
    for (Particle p: pc) {
        p.setV({0, 0, 0});
        EXPECT_TRUE(pcRef.contains(p));
    }
}

/*
 *Part three: Testing the generateMembrane() method:
 */

/*
 *Testing basic functionality.
 */

bool isDiagonalTopLeftNeighbor(Particle &source, Particle &neighbor, double h) {
    return source.getX()[1] + h == neighbor.getX()[1]
           and source.getX()[0] - h == neighbor.getX()[0];
}

bool isDiagonalBottomLeftNeighbor(Particle &source, Particle &neighbor, double h) {
    return source.getX()[1] - h == neighbor.getX()[1]
           and source.getX()[0] - h == neighbor.getX()[0];
}

bool isDirectNeighborOnYAxis(Particle &source, Particle &neighbor, double h) {
    return source.getX()[1] - h == neighbor.getX()[1]
           and source.getX()[0] == neighbor.getX()[0];
}

bool isDirectNeighborOnXAxis(Particle &source, Particle &neighbor, double h) {
    return source.getX()[1] == neighbor.getX()[1]
           and source.getX()[0] - h == neighbor.getX()[0];
}

TEST(ParticleGeneratorTest_Membrane, NumberOfParticlesCorrect) {
    LinkedCellsContainer lcc{{12, 12, 0}, 1, BoundarySet{}};
    ParticleGenerator::generateMembrane(lcc, {1, 1, 0}, 10, 10, 1, 1, {0, 0, 0},
                                        [](unsigned n1, unsigned n2) { return false; });
    EXPECT_EQ(lcc.size(), 100);
}

TEST(ParticleGeneratorTest_Membrane, NeighborsCorrect) {
    DefaultParticleContainer ds;
    ParticleGenerator::generateMembrane(ds, {0.5, 0.5, 0}, 3, 3, 1, 1, {0, 0, 0},
                                        [](unsigned n1, unsigned n2) { return false; });
    ASSERT_EQ(ds.at(0).getDirectNeighbors().size(), 0);
    ASSERT_EQ(ds.at(0).getDiagonalNeighbors().size(), 0);

    ASSERT_EQ(ds.at(1).getDirectNeighbors().size(), 1);
    ASSERT_EQ(ds.at(1).getDiagonalNeighbors().size(), 0);
    EXPECT_EQ(ds.at(1).getDirectNeighbors()[0], 1);

    ASSERT_EQ(ds.at(2).getDirectNeighbors().size(), 1);
    ASSERT_EQ(ds.at(2).getDiagonalNeighbors().size(), 0);
    EXPECT_EQ(ds.at(2).getDirectNeighbors()[0], 2);

    ASSERT_EQ(ds.at(3).getDirectNeighbors().size(), 1);
    ASSERT_EQ(ds.at(3).getDiagonalNeighbors().size(), 1);
    EXPECT_EQ(ds.at(3).getDirectNeighbors()[0], 1);
    EXPECT_EQ(ds.at(3).getDiagonalNeighbors()[0], 2);

    ASSERT_EQ(ds.at(4).getDirectNeighbors().size(), 2);
    ASSERT_EQ(ds.at(4).getDiagonalNeighbors().size(), 2);
    EXPECT_EQ(ds.at(4).getDirectNeighbors()[0], 2);
    EXPECT_EQ(ds.at(4).getDirectNeighbors()[1], 4);
    EXPECT_EQ(ds.at(4).getDiagonalNeighbors()[0], 1);
    EXPECT_EQ(ds.at(4).getDiagonalNeighbors()[1], 3);

    ASSERT_EQ(ds.at(5).getDirectNeighbors().size(), 2);
    ASSERT_EQ(ds.at(5).getDiagonalNeighbors().size(), 1);
    EXPECT_EQ(ds.at(5).getDirectNeighbors()[0], 3);
    EXPECT_EQ(ds.at(5).getDirectNeighbors()[1], 5);
    EXPECT_EQ(ds.at(5).getDiagonalNeighbors()[0], 2);

    ASSERT_EQ(ds.at(6).getDirectNeighbors().size(), 1);
    ASSERT_EQ(ds.at(6).getDiagonalNeighbors().size(), 1);
    EXPECT_EQ(ds.at(6).getDirectNeighbors()[0], 4);
    EXPECT_EQ(ds.at(6).getDiagonalNeighbors()[0], 5);

    ASSERT_EQ(ds.at(7).getDirectNeighbors().size(), 2);
    ASSERT_EQ(ds.at(7).getDiagonalNeighbors().size(), 2);
    EXPECT_EQ(ds.at(7).getDirectNeighbors()[0], 5);
    EXPECT_EQ(ds.at(7).getDirectNeighbors()[1], 7);
    EXPECT_EQ(ds.at(7).getDiagonalNeighbors()[0], 4);
    EXPECT_EQ(ds.at(7).getDiagonalNeighbors()[1], 6);

    ASSERT_EQ(ds.at(8).getDirectNeighbors().size(), 2);
    ASSERT_EQ(ds.at(8).getDiagonalNeighbors().size(), 1);
    EXPECT_EQ(ds.at(8).getDirectNeighbors()[0], 6);
    EXPECT_EQ(ds.at(8).getDirectNeighbors()[1], 8);
    EXPECT_EQ(ds.at(8).getDiagonalNeighbors()[0], 5);

}
