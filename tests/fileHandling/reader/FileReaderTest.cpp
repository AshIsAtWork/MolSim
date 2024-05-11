//
// Created by daniel on 11.05.24.
//

#include <gtest/gtest.h>
#include "fileHandling/reader/FileReader.h"
#include <spdlog/spdlog.h>

/**
 * Set up test environment for first test suite.
 */
class FileReaderTest: public testing::Test {


protected:
    ParticleContainer pc;

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }
};

/**
 * Is the file read correctly?
 * Note that this can be only tested indirectly by checking, if the right particles are created, because the
 * read method calls the generateCuboid function directly after reading the parameters.
 */

TEST_F(FileReaderTest, CuboidFormat_OneCuboid) {
    FileReader::readFile(pc, "../tests/testData/CuboidTest1.txt");
    ASSERT_EQ(pc.size(), 2);
    Particle p0 = pc.at(0);
    Particle p1 = pc.at(1);
    std::array<double, 3> pos_p0 = {0,0,0};
    std::array<double, 3> pos_p1 = {0,0,1};
    EXPECT_TRUE(p0.getX() == pos_p0);
    EXPECT_TRUE(p1.getX() == pos_p1);
    EXPECT_EQ(p0.getM(),2.0);
    EXPECT_EQ(p1.getM(),2.0);
}

/**
 * Test Cuboid format file that contains multiple cuboids
 */

TEST_F(FileReaderTest, CuboidFormat_MultipleCuboids) {
    FileReader::readFile(pc, "../tests/testData/CuboidTest2.txt");
    ASSERT_EQ(pc.size(), 5);
    Particle p0 = pc.at(0);
    Particle p1 = pc.at(1);
    Particle p2 = pc.at(2);
    Particle p3 = pc.at(3);
    Particle p4 = pc.at(4);;
    std::array<double, 3> pos_p0 = {0,0,0};
    std::array<double, 3> pos_p1 = {1,0,0};
    std::array<double, 3> pos_p2 = {2,0,0};
    std::array<double, 3> pos_p3 = {3,0,0};
    std::array<double, 3> pos_p4 = {4,0,0};
    EXPECT_TRUE(p0.getX() == pos_p0);
    EXPECT_TRUE(p1.getX() == pos_p1);
    EXPECT_TRUE(p2.getX() == pos_p2);
    EXPECT_TRUE(p3.getX() == pos_p3);
    EXPECT_TRUE(p4.getX() == pos_p4);
    EXPECT_EQ(p0.getM(),1.0);
    EXPECT_EQ(p1.getM(),2.0);
    EXPECT_EQ(p2.getM(),3.0);
    EXPECT_EQ(p3.getM(),4.0);
    EXPECT_EQ(p4.getM(),5.0);
}

/**
 * Is the file read correctly?
 * Note that this can be only tested indirectly by checking, if the right particles are created, because the
 * read method calls the generateCuboid function directly after reading the parameters.
 */

TEST_F(FileReaderTest, ParticleFormat_OneParticle) {
    FileReader::readFile(pc, "../tests/testData/ParticleTest1.txt");
    ASSERT_EQ(pc.size(), 1);
    Particle p = pc.at(0);
    std::array<double, 3> pos = {1,2,3};
    std::array<double, 3> vel = {4,5,6};
    EXPECT_TRUE(p.getX() == pos);
    EXPECT_TRUE(p.getV() == vel);
    EXPECT_EQ(p.getM(),7.0);
}

/**
 * Test Particle format file that contains multiple particles
 */

TEST_F(FileReaderTest, ParticleFormat_MultipleParticles) {
    FileReader::readFile(pc, "../tests/testData/ParticleTest2.txt");
    ASSERT_EQ(pc.size(), 2);
    Particle p0 = pc.at(0);
    Particle p1 = pc.at(1);

    std::array<double, 3> pos_p0 = {10,20,30};
    std::array<double, 3> pos_p1 = {100,200,300};
    std::array<double, 3> vel_p0 = {40,50,60};
    std::array<double, 3> vel_p1 = {400,500,600};
    EXPECT_TRUE(p0.getX() == pos_p0);
    EXPECT_TRUE(p1.getX() == pos_p1);
    EXPECT_TRUE(p0.getV() == vel_p0);
    EXPECT_TRUE(p1.getV() == vel_p1);
    EXPECT_EQ(p0.getM(),70.0);
    EXPECT_EQ(p1.getM(),700.0);
}



