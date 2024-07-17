//
// Created by daniel on 01.07.24.
//

#include <gtest/gtest.h>

#include "moleculeSimulator/statistics/Statistics.h"
#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"

/**
 * Compare computed velocity and density profiles against hand calculated values.
 */

TEST(StatisticsTest, calculateVelocityProfile) {
    {
        Statistics statistics{3};
        DefaultParticleContainer dpc;
        Particle secondBin1{{4.3,1,0},{1,-10,5},4.5};
        Particle secondBin2{{5.5,1,0},{1,-20,5},4.5};
        Particle thirdBin1{{8.9,1,0},{1,-100,5},1.5};
        Particle thirdBin2{{7.5,1,0},{1,-50,5},1.5};
        Particle thirdBin_fixed{{8.1,1,0},{1,-30,5},1};
        thirdBin_fixed.setFixed(true);
        dpc.add(secondBin1);
        dpc.add(secondBin2);
        dpc.add(thirdBin1);
        dpc.add(thirdBin2);
        dpc.add(thirdBin_fixed);
        statistics.calculateVelocityAndDensityProfile(dpc, {9,1,1}, 0);
    }

    //Check velocity profile
    std::ifstream file;

    file = std::ifstream("../cmake-build-debug/statistics/VelocityProfile.csv");
    if(!file.is_open()) {
        file = std::ifstream("../build/statistics/VelocityProfile.csv");
    }

    ASSERT_TRUE(file.is_open());
    std::string line;
    //Skip first line
    getline(file, line);
    //Read line with values
    line.clear();
    getline(file, line);
    //Clear commas
    for(char & i : line) {
        if(i == ',') {
            i = ' ';
        }
    }
    std::istringstream numstream(line);
    int num;
    numstream >> num;
    EXPECT_EQ(num, 0);
    numstream >> num;
    EXPECT_EQ(num, 0);
    numstream >> num;
    EXPECT_EQ(num, -15);
    numstream >> num;
    EXPECT_EQ(num, -75);

    //Check densities

    std::ifstream file2;

    file2 = std::ifstream("../cmake-build-debug/statistics/DensityProfile.csv");
    if(!file2.is_open()) {
        file2 = std::ifstream("../build/statistics/DensityProfile.csv");
    }

    ASSERT_TRUE(file2.is_open());
    std::string line2;
    //Skip first line
    getline(file2, line2);
    //Read line with values
    line2.clear();
    getline(file2, line2);
    //Clear commas
    for(char & i : line2) {
        if(i == ',') {
            i = ' ';
        }
    }
    std::istringstream numstream2(line2);
    double num2;
    numstream2 >> num2;
    EXPECT_DOUBLE_EQ(num2, 0);
    numstream2 >> num2;
    EXPECT_DOUBLE_EQ(num2, 0);
    numstream2 >> num2;
    EXPECT_DOUBLE_EQ(num2, 3.0);
    numstream2 >> num2;
    EXPECT_DOUBLE_EQ(num2, 1.0);
}