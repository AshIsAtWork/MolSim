//
// Created by daniel on 24.04.24.
//

#pragma once

#include "fileHandling/FileHandler.h"
#include "fileHandling/outputWriter/VTKWriter/VTKWriter.h"
#include "forceCalculation/Force.h"
#include "models/Model.h"
#include "utils/enumsStructs.h"
#include <memory>
#include <moleculeSimulator/thermostats/DefaultThermostat.h>
#include "forceCalculation/gravity/Gravity.h"
#include "forceCalculation/leonardJones/LeonardJonesForce.h"
#include "models/directSum/DirectSum.h"
#include "../models/linkedCells/LinkedCells.h"

/**
 * @brief This class implements the simulation of the particle system.
 *
 * Using Newton's axioms and the Störmer-Verlet equations force, velocity and position of all particles are computed
 * in fixed discrete time steps. Results are written to files.
 */

class Simulator {
private:
 //thermostat
 std::unique_ptr<DefaultThermostat> thermostat;
 int nThermostat;
 bool useThermostat;
 bool initialiseSystemWithBrownianMotion;
 bool applyScalingGradually;

 //simulation dependent
 std::unique_ptr<Force> force;
 std::unique_ptr<Model> model;
 double deltaT;
 double endT;

 //output
 int outputFrequency;
 std::string outputFileBaseName;

 //performance measurements
 unsigned long long totalMoleculeUpdates;

public:
 Simulator() = delete;


 /**
  * @brief Constructor to contruct a new simulation evironment.
  *
  * @param simulationSettings Specify all parameters of the simulation environment.
  * @param outputFormat Format of the output files.
  */
 Simulator(SimulationSettings &simulationSettings, FileHandler::outputFormat outputFormat);

 /**
  * @brief Legacy constructor to construct a new simulation environment using the direct sum algorithm.
  *
  * @param parameters Simulation parameters for the direct sum model.
  * @param inputFilePath Path to the input file which comprises the particles going to be simulated.
  * @param outputFormat Format of the output file. Supported formats are vtk and xyz.
  * @param outputFrequency Specifies after how much time steps an output file is written. For example an output frequency
  *                        of 10 means that after each 10 iterations an output file is written.
  * @param outputFileBaseName Base name of the output files.
  *
  * To create a new simulation environment you have to provide an input file containing the particles you want
  * to simulate.
  */

 Simulator(DirectSumSimulationParameters &parameters, std::string &inputFilePath,
           FileHandler::outputFormat outputFormat, int outputFrequency,
           std::string &outputFileBaseName);

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
  * @brief Load the state of all molecules from a previous simulation back into this simulation.
  *
  * @param pathToMolecules Path pointing to the txt file which stores the state of the molecules.
  */
 void loadState(std::string& pathToMolecules);

 /**
  * @brief Export the current state of all molecules to a txt file for using them in a new simulation.
  */
 void saveState();

 /**
  * @brief Get the Particle container of this simulator
  *
  * @return Particle container of this simulator
  */

 ParticleContainer &getParticles();

 unsigned long long getTotalMoleculeUpdates();

};
