//
// Created by daniel on 01.07.24.
//

#include <gtest/gtest.h>

#include "moleculeSimulator/statistics/Statistics.h"
#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"

/**
 * Compare computed velocity profile against hand calculated values.
 */

TEST(StatisticsTest, calculateVelocityProfile) {
    {
        Statistics statistics{3};
        DefaultParticleContainer dpc;
        Particle secondBin1{{4.3,1,0},{1,-10,5},1};
        Particle secondBin2{{5.5,1,0},{1,-20,5},1};
        Particle thirdBin1{{8.9,1,0},{1,-100,5},1};
        Particle thirdBin2{{7.5,1,0},{1,-50,5},1};
        Particle thirdBin_fixed{{8.1,1,0},{1,-30,5},1};
        thirdBin_fixed.setFixed(true);
        dpc.add(secondBin1);
        dpc.add(secondBin2);
        dpc.add(thirdBin1);
        dpc.add(thirdBin2);
        dpc.add(thirdBin_fixed);
        statistics.calculateVelocityAndDensityProfile(dpc, 9, 0);
    }
    std::ifstream file;

    file = std::ifstream("../cmake-build-debug/statistics/Profile.csv");
    if(!file.is_open()) {
        file = std::ifstream("../build/statistics/Profile.csv");
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
    EXPECT_EQ(num, -60);
}