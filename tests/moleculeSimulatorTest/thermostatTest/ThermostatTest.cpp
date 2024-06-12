//
// Created by daniel on 10.06.24.
//

#include <complex>
#include <gtest/gtest.h>

#include "models/directSum/DirectSum.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"
#include "moleculeSimulator/thermostat/Thermostat.h"

class ThermostatTest : public testing::Test {
protected:
    LeonardJonesForce lJF;
    DirectSum dS = {lJF, 0.1, FileHandler::outputFormat::vtk};

    void SetUp() override {
        //Deactivate all console output
        spdlog::set_level(spdlog::level::off);
    }
};

TEST_F(ThermostatTest, calculateKineticEnergy_BasicTest) {
    Thermostat thermostat{dS, 10, 10, 10, 3};

    //Kinetic energy should be 0 when there are no particles
    ASSERT_EQ(thermostat.calculateKineticEnergy(),0);

    //Add a couple of particles
    Particle p1{{0,0,0},{1,1,1},1};
    Particle p2{{0,0,0},{2,2,2},2};
    Particle p3{{0,0,0},{3,3,3},3};
    Particle p4{{0,0,0},{-1,-1,-1},1};

    dS.addParticle(p1);
    dS.addParticle(p2);
    dS.addParticle(p3);
    dS.addParticle(p4);

    EXPECT_EQ(thermostat.calculateKineticEnergy(),55.5);
}

TEST_F(ThermostatTest, calculateTemperature_BasicTest) {
    Thermostat thermostat{dS, 10, 10, 10, 3};

    //Add a couple of particles
    Particle p1{{0,0,0},{1,1,1},1};
    Particle p2{{0,0,0},{2,2,2},2};
    Particle p3{{0,0,0},{3,3,3},3};
    Particle p4{{0,0,0},{-1,-1,-1},1};

    dS.addParticle(p1);
    dS.addParticle(p2);
    dS.addParticle(p3);
    dS.addParticle(p4);

    EXPECT_EQ(thermostat.calculateKineticEnergy(),55.5);
}

TEST_F(ThermostatTest, calculateTemperature_UndefinedTemperature) {
    Thermostat thermostat{dS, 10, 10, 10, 3};
    EXPECT_DEATH(thermostat.calculateTemperature(),"");
}

TEST_F(ThermostatTest, initialiseSystem_BasicTest) {
    Thermostat thermostat{dS, 100, 10, 100, 3};
    //Add a bunch of particles

    for(int i = 0; i < 10000; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        dS.addParticle(p);
    }
    thermostat.initialiseSystem();
    //Because the Brownian Motion is applied random, it is not guarranteed that the temperature of the system is
    //exactly 100. Nevertheless it should be close to this value, because the system comprises 10000 particles.
    EXPECT_TRUE(abs(thermostat.calculateTemperature() - 100) < 1);
}

TEST_F(ThermostatTest, setTemperatureOfTheSystemViaVelocityScaling_BasisTest) {
    Thermostat thermostat{dS, 1, 100, 100, 3};

    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        dS.addParticle(p);
    }

    //Initialise system

    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 100.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();

    EXPECT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);

}

TEST_F(ThermostatTest, setTemperatureOfTheSystemViaGradualVelocityScaling_BasicTest) {
    Thermostat thermostat{dS, 1, 10, 10, 3};


    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
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
}

//The following tests, test cooling, heating and holding a temperature.

TEST_F(ThermostatTest, cooling) {

    Thermostat thermostat{dS, 1, 100, 1, 3};


    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        dS.addParticle(p);
    }

    //Initialise system
    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 10.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();
    ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);

    //Set target temperature to 1 and cool down the system to this value in excactly 99 steps
    thermostat.setTargetTemperature(1);

    for(int i = 0; i < 99; i++) {
        thermostat.setTemperatureOfTheSystemViaGradualVelocityScaling();
        ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - (99 - i)) <= 1e-9);
    }
    EXPECT_TRUE(std::abs(thermostat.calculateTemperature() - 1) <= 1e-9);
}

TEST_F(ThermostatTest, heating) {

    Thermostat thermostat{dS, 1, 1, 1, 3};


    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        dS.addParticle(p);
    }

    //Initialise system
    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 100.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();
    ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 1) <= 1e-9);

    //Set target temperature to 100 and heat up the system to this value in excactly 99 steps
    thermostat.setTargetTemperature(100);

    for(int i = 0; i < 99; i++) {
        thermostat.setTemperatureOfTheSystemViaGradualVelocityScaling();
        ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - (i+2)) <= 1e-9);
    }
    EXPECT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);
}

TEST_F(ThermostatTest, holdingATemperature) {

    Thermostat thermostat{dS, 100, 100, 1, 3};

    //Add a bunch of particles
    for(int i = 0; i < 100; i++) {
        Particle p{{0,0,0},{0,0,0},1};
        dS.addParticle(p);
    }

    //Initialise system
    thermostat.initialiseSystem();

    //Set temperature of the system to exactly 100.

    thermostat.setTemperatureOfTheSystemViaVelocityScaling();
    ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);

    //We are now manipulating the velocity off all particles with Brownian Motion. The system should nevertheless keep its temperature of 100.

    for(int i = 0; i < 100; i++) {
        //Manipulate temperature
        dS.getParticles().applyToEachParticleInDomain([](Particle& p) {
            p.setV(p.getV() + maxwellBoltzmannDistributedVelocity(0.5, 3));
        });
        //Restore target temperature
        thermostat.setTemperatureOfTheSystemViaVelocityScaling();
        ASSERT_TRUE(std::abs(thermostat.calculateTemperature() - 100) <= 1e-9);
    }

}




