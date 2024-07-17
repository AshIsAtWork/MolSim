//
// Created by daniel on 01.07.24.
//

#include <gtest/gtest.h>

#include "models/directSum/DirectSum.h"
#include "moleculeSimulator/forceCalculation/lennardJones/LennardJonesForce.h"
#include "moleculeSimulator/thermostats/flowThermostat/FlowThermostat.h"

class FlowThermostatTest : public testing::Test {
protected:
    LennardJonesForce lJF;
    DirectSum dS = {lJF, 0.1, FileHandler::outputFormat::vtk, false};

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }
};

/**
 * Compare calculated velocity against hand calculated value.
 */
TEST_F(FlowThermostatTest, averageVelocityTest) {
    FlowThermostat thermostat{dS, 10, 10, 10, 3};

    //Add a couple of particles
    Particle p1{{0,0,0},{1,1,1},1};
    Particle p2{{0,0,0},{2,2,2},2};
    Particle p3{{0,0,0},{3,3,3},3};
    Particle p4{{0,0,0},{-1,-1,-1},1};

    dS.addParticle(p1);
    dS.addParticle(p2);
    dS.addParticle(p3);
    dS.addParticle(p4);

    auto result = thermostat.calculateAverageVelocity();

    EXPECT_DOUBLE_EQ(result[0],1.25);
    EXPECT_DOUBLE_EQ(result[1],1.25);
    EXPECT_DOUBLE_EQ(result[2],1.25);
}

/**
 * Check, if fixed particles are ignored.
 */
TEST_F(FlowThermostatTest, averageVelocityTest_FixedParticles) {
    FlowThermostat thermostat{dS, 10, 10, 10, 3};

    //Add a couple of particles
    Particle p1{{0,0,0},{1,1,1},1};
    Particle p2{{0,0,0},{2,2,2},2};
    Particle p3{{0,0,0},{3,3,3},3};
    Particle p4{{0,0,0},{-1,-1,-1},1};
    p4.setFixed(true);

    dS.addParticle(p1);
    dS.addParticle(p2);
    dS.addParticle(p3);
    dS.addParticle(p4);

    auto result = thermostat.calculateAverageVelocity();

    EXPECT_DOUBLE_EQ(result[0],2);
    EXPECT_DOUBLE_EQ(result[1],2);
    EXPECT_DOUBLE_EQ(result[2],2);
}

/**
 * The average velocity of a system with no particles is not defined.
 */
TEST_F(FlowThermostatTest, averageVelocityTest_ZeroParticles) {
    FlowThermostat thermostat{dS, 10, 10, 10, 3};
    EXPECT_THROW(thermostat.calculateAverageVelocity(), std::exception);
}

/**
 * Compare calculated kinetic energy against hand calculated values.
 */
TEST_F(FlowThermostatTest, calculateKineticEnergy) {
    FlowThermostat thermostat{dS, 10, 10, 10, 3};

    Particle p1{{0,0,0},{1,1,1},1};
    Particle p2{{0,0,0},{2,2,2},2};
    Particle p3{{0,0,0},{3,3,3},3};
    Particle p4{{0,0,0},{-1,-1,-1},1};
    p4.setFixed(true);

    dS.addParticle(p1);
    dS.addParticle(p2);
    dS.addParticle(p3);
    dS.addParticle(p4);

    EXPECT_DOUBLE_EQ(thermostat.calculateKineticEnergy(), 6);
}

/**
 * Check, if the systems temperature after initialisation is close to the demanded value temperature init.
 */
TEST_F(FlowThermostatTest, initialiseSystem) {
    FlowThermostat thermostat{dS, 100, 10, 100, 3};
    //Add a bunch of particles

    for(int i = 0; i < 10000; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        if(i % 10 == 0) {
            p.setFixed(true);
        }
        dS.addParticle(p);
    }
    thermostat.initialiseSystem();
    //Because the Brownian Motion is applied random, it is not guarranteed that the temperature of the system is
    //exactly 100. Nevertheless it should be close to this value, because the system comprises 10000 particles.
    EXPECT_TRUE(abs(thermostat.calculateTemperature() - 100) < 1);

    //Fixed particles should have a velocity of zero

    std::array<double, 3> expected = {0,0,0};
    dS.getParticles().applyToEachParticleInDomain([expected](Particle& p) {
        if(p.isFixed()) {
            EXPECT_TRUE(expected == p.getV());
        }
    });

}

/**
 * Check, if temperature set via velocity scaling immediately corresponds to the target temperature
 */
TEST_F(FlowThermostatTest, setTemperatureOfTheSystemViaVelocityScaling) {
    FlowThermostat thermostat{dS, 1, 100, 100, 3};

    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        if(i % 10 == 0) {
            p.setFixed(true);
        }
        dS.addParticle(p);
    }

    //Initialise system

    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 100.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();

    EXPECT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);

    //Fixed particles should have a velocity of zero

    std::array<double, 3> expected = {0,0,0};
    dS.getParticles().applyToEachParticleInDomain([expected](Particle& p) {
        if(p.isFixed()) {
            EXPECT_TRUE(expected == p.getV());
        }
    });

}

/**
 * Check, if temperature set via gradual velocity scaling reaches the target temperature in 10 steps. Each step the
 * systems temperature should be increased by 1.
 */
TEST_F(FlowThermostatTest, setTemperatureOfTheSystemViaGradualVelocityScaling) {
    FlowThermostat thermostat{dS, 1, 10, 10, 3};


    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        if(i % 10 == 0) {
            p.setFixed(true);
        }
        dS.addParticle(p);
    }

    //Initialise system
    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 10.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();
    ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 10) <= 1e-9);

    //Set target temperature to 100 and increase temperature of the system it in exactly 9 steps to this value
    thermostat.setTargetTemperature(100);

    for(int i = 0; i < 9; i++) {
        thermostat.setTemperatureOfTheSystemViaGradualVelocityScaling();
        ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - (i+2)*10) <= 1e-9);
    }

    ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);

    //Fixed particles should have a velocity of zero

    std::array<double, 3> expected = {0,0,0};
    dS.getParticles().applyToEachParticleInDomain([expected](Particle& p) {
        if(p.isFixed()) {
            EXPECT_TRUE(expected == p.getV());
        }
    });
}




