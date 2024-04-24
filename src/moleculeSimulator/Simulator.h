//
// Created by daniel on 24.04.24.
//

#pragma once

#include "fileHandling/FileHandler.h"
#include "fileHandling/outputWriter/VTKWriter.h"
#include "ForceCalculation/Gravity.h"
#include "particleRepresentation/ParticleContainer.h"

class Simulator {

private:

    FileHandler fileHandler;
    ParticleContainer particles;
    Gravity gravity;
    double deltaT;
    double endT;

    /**
    * calculate the force for all particles
    */

    void calculateF();

    /**
    * calculate the position for all particles
    */

    void calculateX();

    /**
    * calculate the position for all particles
    */

    void calculateV();


public:
    Simulator() = delete;
    explicit Simulator(std::string& inputFilePath);

    void configure(double endT, double deltaT);

    void run();

    ParticleContainer getParticles();
};
