//
// Created by daniel on 24.04.24.
//

#pragma once

#include "fileHandling/FileHandler.h"
#include "fileHandling/outputWriter/VTKWriter.h"
#include <iostream>
#include "forceCalculation/Force.h"
#include "models/Model.h"

/**
 * @brief This class implements the simulation of the particle system.
 *
 * Using Newton's axioms and the Störmer-Verlet equations force, velocity and position of all particles are computed
 * in fixed discrete time steps. Results are written to files.
 */

class Simulator {
private:
    Model &model;
    Force &force;
    double deltaT;
    double endT;
    FileHandler::inputFormat inputFormat;
    FileHandler::outputFormat outputFormat;


public:
    Simulator() = delete;

    /**
     * @brief Construct a new simulation environment.
     *
     * @param inputFilePath Path to the input file which comprises the particles going to be simulated.
     * @param model Model of the particles that is used within in simulation
     * @param force Type of force to be used in the simulation
     * @param endT Time to which the simulation is going to run.
     * @param deltaT Duration of one time step. Small time step will result in a better simulation, but will demand more computational resources.
     * @param inputFormat Format of the input file. Supported formats are txt and xml.
     * @param outputFormat Format of the output file. Supported formats are vtk and xyz.
     *
     * To create a new simulation environment you have to provide an input file containing the particles you want
     * to simulate.
     */
    explicit Simulator(std::string &inputFilePath, Force &force, double endT, double deltaT,
                       FileHandler::inputFormat inputFormat, FileHandler::outputFormat outputFormat);

    Simulator(std::string &inputFilePath, Model &model, Force &force, double endT, double deltaT);

    /**
     * @brief Run the simulation.
     *
     * After configuration of the parameters the simulation can be run calling this method. When no configuration is done before,
     * the default parameters will be used.
     *
     * @param benchmark Activate or deactivate time measurement
     */

    void run(bool benchmark);

    /**
     * Get the Particle container of this simulator
     *
     * @return Particle container of this simulator
     */

    ParticleContainer &getParticles();
};
