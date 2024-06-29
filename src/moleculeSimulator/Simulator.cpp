//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"

#include <iostream>

using namespace enumsStructs;

Simulator::Simulator(SimulationSettings &simulationSettings, FileHandler::outputFormat outputFormat) {
    //Set model independent parameters
    outputFrequency = simulationSettings.outputFrequency;
    outputFileBaseName = simulationSettings.outputFileName;

    //Set model dependent parameters
    switch (simulationSettings.model) {
        case TypeOfModel::directSum: {
            switch (simulationSettings.parametersDirectSum.force) {
                case TypeOfForce::gravity: {
                    force = std::make_unique<Gravity>();
                }
                break;
                case TypeOfForce::leonardJonesForce: {
                    force = std::make_unique<LeonardJonesForce>();
                }
                break;
                default: {
                    throw std::invalid_argument("Invalid force used.");
                }
            }
            deltaT = simulationSettings.parametersDirectSum.deltaT;
            endT = simulationSettings.parametersDirectSum.endT;
            model = std::make_unique<DirectSum>(*force, simulationSettings.parametersDirectSum.deltaT, outputFormat,
                                                simulationSettings.gravityOn, simulationSettings.gravityVector);
        }
        break;
        case TypeOfModel::linkedCells: {
            switch (simulationSettings.parametersLinkedCells.force) {
                case TypeOfForce::gravity: {
                    force = std::make_unique<Gravity>();
                }
                break;
                case TypeOfForce::leonardJonesForce: {
                    force = std::make_unique<LeonardJonesForce>();
                }
                break;
                default: {
                    throw std::invalid_argument("Invalid Force type is used.");
                }
            }
            deltaT = simulationSettings.parametersLinkedCells.deltaT;
            endT = simulationSettings.parametersLinkedCells.endT;

            model = std::make_unique<LinkedCells>(*force, simulationSettings.parametersLinkedCells.deltaT,
                                                  simulationSettings.parametersLinkedCells.domainSize,
                                                  simulationSettings.parametersLinkedCells.rCutOff,
                                                  outputFormat,
                                                  simulationSettings.parametersLinkedCells.boundaryConditions,
                                                  simulationSettings.gravityOn,
                                                  simulationSettings.gravityVector, simulationSettings.membraneParameters);
        }
        break;
        default: {
            throw std::invalid_argument("Invalid Model type used");
        }
    }

    //Set thermostat, if it is used
    useThermostat = simulationSettings.thermostatParameters.useThermostat;

    if (useThermostat) {
        applyScalingGradually = simulationSettings.thermostatParameters.applyScalingGradually;
        nThermostat = simulationSettings.thermostatParameters.applyAfterHowManySteps;
        initialiseSystemWithBrownianMotion = simulationSettings.thermostatParameters.initialiseSystemWithBrownianMotion;
        thermostat = std::make_unique<Thermostat>(*model, simulationSettings.thermostatParameters.initialTemperature,
                                                  simulationSettings.thermostatParameters.targetTemperature,
                                                  simulationSettings.thermostatParameters.maxTemperatureChange,
                                                  simulationSettings.thermostatParameters.dimensions);
    } else {
        applyScalingGradually = false;
        nThermostat = INT32_MAX;
        initialiseSystemWithBrownianMotion = false;
    }

    //Add particles and objects of particles

    //Particles
    for (auto pT: simulationSettings.particles) {
        Particle p{pT.x, pT.v, pT.m, 1,pT.epsilon, pT.sigma};
        model->addParticle(p);
    }
    //Cuboids
    for (auto cuboid: simulationSettings.cuboids) {
        model->addCuboid(cuboid.position, cuboid.dimensions[0], cuboid.dimensions[1], cuboid.dimensions[2], cuboid.h,
                         cuboid.mass, cuboid.initVelocity, cuboid.dimensionsBrownianMotion,
                         cuboid.brownianMotionAverageVelocity, cuboid.epsilon, cuboid.sigma);
    }
    //Discs
    for (auto disc: simulationSettings.discs) {
        model->addDisc(disc.center, disc.initVelocity, disc.N, disc.h, disc.mass, disc.dimensionsBrownianMotion,
                       disc.brownianMotionAverageVelocity, disc.epsilon, disc.sigma);
    }

    totalMoleculeUpdates = 0;
}

Simulator::Simulator(DirectSumSimulationParameters &parameters, std::string &inputFilePath,
                     FileHandler::outputFormat outputFormat,
                     int outputFrequency, std::string &outputFileBaseName) : deltaT{parameters.deltaT},
                                                                             endT{parameters.endT},
                                                                             outputFrequency{outputFrequency},
                                                                             outputFileBaseName{outputFileBaseName} {
    switch (parameters.force) {
        case TypeOfForce::gravity: {
            force = std::make_unique<Gravity>();
        }
        break;
        case TypeOfForce::leonardJonesForce: {
            force = std::make_unique<LeonardJonesForce>();
        }
        break;
        default: {
            throw std::invalid_argument("Invalid Force Type");
        }
    }
    model = std::make_unique<DirectSum>(*force, parameters.deltaT, outputFormat, false);
    model->addViaFile(inputFilePath, FileHandler::inputFormat::txt);

    //Thermostat is not used
    useThermostat = false;
    applyScalingGradually = false;
    nThermostat = INT32_MAX;
    initialiseSystemWithBrownianMotion = false;
    totalMoleculeUpdates = 0;
}

void Simulator::run(bool benchmark) {
    double current_time = 0;
    int iteration = 0;

    //Set the temperature of the system if specified
    if (initialiseSystemWithBrownianMotion) {
        thermostat->initialiseSystem();
    }

    //Plot everything one time before the simulation starts.
    if (!benchmark) {
        model->plot(iteration, outputFileBaseName);
    }

    //Calculate the initial forces before starting the simulation
    model->initializeForces();


    while (current_time < endT) {

        //Count, how much molecules will be updated in total.
        if(benchmark) {
            totalMoleculeUpdates += model->getParticles().size();
        }

        //Do one simulation step
        model->step(iteration);

        //Control temperature if thermostat is specified
        if (useThermostat && iteration % nThermostat == 0) {
            if (applyScalingGradually) {
                thermostat->setTemperatureOfTheSystemViaGradualVelocityScaling();
            } else {
                thermostat->setTemperatureOfTheSystemViaVelocityScaling();
            }
        }

        iteration++;
        if (!benchmark && iteration % outputFrequency == 0) {
            model->plot(iteration, outputFileBaseName);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

void Simulator::loadState(std::string &pathToMolecules) {
    int particlesBefore = model->getParticles().size();
    model->addViaFile(pathToMolecules,FileHandler::inputFormat::txt);
    spdlog::info("Loaded {} molecules into the simulation", model->getParticles().size() - particlesBefore);
}

void Simulator::saveState() {
    model->saveState();
}

ParticleContainer &Simulator::getParticles() {
    return model->getParticles();
}

unsigned long long Simulator::getTotalMoleculeUpdates() {
    return totalMoleculeUpdates;
}
