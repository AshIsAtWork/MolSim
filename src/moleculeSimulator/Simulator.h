//
// Created by daniel on 24.04.24.
//

#pragma once

#include "fileHandling/FileHandler.h"
#include "fileHandling/outputWriter/VTKWriter.h"
#include "forceCalculation/gravity/Gravity.h"
#include "particleRepresentation/ParticleContainer.h"
#include "utils/ArrayUtils.h"

#include <iostream>

/**
 * @brief This class implements the simulation of the particle system.
 *
 * Using Newton's axioms and the Störmer-Verlet equations force, velocity and position of all particles are computed
 * in fixed discrete time steps. Results are written to files.
 */

class Simulator {

private:

    FileHandler fileHandler;
    ParticleContainer particles;
    Force &force;
    double deltaT;
    double endT;

    /**
    * @brief Helper method to calculate the force of all particles.
    *
    * After each time step the forces acting between the particles have changed due to their new positions, so
    * they have to be recalculated. This method uses Newtons third law of motion to simplify calculations and make
    * them more efficient.
    */

    void calculateF();

    /**
    * @brief Depreciated helper method to calculate the force of all particles.
    *
    * After each time step the forces acting between the particles have changed due to their new positions, so
    * they have to be recalculated. This method iterates naively over all pairs of particles and is only their for
    * comparing time measurements.
    */

    void calculateF_naive();
    /**
    * @brief Helper method to calculate the position of all particles.
    *
    * After each time step positions of all particles with velocities unequal to zero have to be updated.
    */

    void calculateX();

    /**
    * @brief Helper method to calculate the velocity of all particles.
    *
    * After each time step the velocity of each particle may have changed due to forces that act between the particles.
    */
    void calculateV();


public:
    Simulator() = delete;

    /**
     * @brief Construct a new simulation environment.
     *
     * @param inputFilePath Path to the input file which comprises the particles going to be simulated.
     * @param force Type of force to be used in the simulation
     * @param endT Time to which the simulation is going to run.
     * @param deltaT Duration of one time step. Small time step will result in a better simulation, but will demand more computational resources.
     *
     * To create a new simulation environment you have to provide an input file containing the particles you want
     * to simulate. After setting the environment all parameters may be adjusted using the method configure.
     */
    explicit Simulator(std::string &inputFilePath, Force &force, double endT, double deltaT);


    /**
     * @brief Run the simulation.
     *
     * After configuration of the parameters the simulation can be run calling this method. When no configuration is done before,
     * the default parameters will be used.
     */
    void run();

};
