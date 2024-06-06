//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"

using namespace enumsStructs;

Simulator::Simulator(SimulationSettings &simulationSettings, FileHandler::inputFormat inputFormat,
                     FileHandler::outputFormat outputFormat) {
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
                    force = std::make_unique<LeonardJonesForce>(simulationSettings.parametersDirectSum.epsilon,
                                                                simulationSettings.parametersDirectSum.sigma);
                }
                break;
                default: {
                    spdlog::error("Use of invalid force type! Programm will be terminated!");
                    exit(-1);
                }
            }
            deltaT = simulationSettings.parametersDirectSum.deltaT;
            endT = simulationSettings.parametersDirectSum.endT;
            model = std::make_unique<DirectSum>(*force, simulationSettings.parametersDirectSum.deltaT, inputFormat,
                                                outputFormat);
        }
        break;
        case TypeOfModel::linkedCells: {
            switch (simulationSettings.parametersLinkedCells.force) {
                case TypeOfForce::gravity: {
                    force = std::make_unique<Gravity>();
                }
                break;
                case TypeOfForce::leonardJonesForce: {
                    force = std::make_unique<LeonardJonesForce>(simulationSettings.parametersLinkedCells.epsilon,
                                                                simulationSettings.parametersLinkedCells.sigma);
                }
                break;
                default: {
                    spdlog::error("Use of invalid force type! Programm will be terminated!");
                    exit(-1);
                }
            }
            deltaT = simulationSettings.parametersLinkedCells.deltaT;
            endT = simulationSettings.parametersLinkedCells.endT;
            model = std::make_unique<LinkedCells>(*force, simulationSettings.parametersLinkedCells.deltaT,
                                                  simulationSettings.parametersLinkedCells.domainSize,
                                                  simulationSettings.parametersLinkedCells.rCutOff,
                                                  simulationSettings.parametersLinkedCells.sigma, inputFormat,
                                                  outputFormat,
                                                  simulationSettings.parametersLinkedCells.boundarySettings);
        }
        break;
        default: {
            spdlog::error("Use of invalid model type! Programm will be terminated!");
            exit(-1);
        }
    }

    //Add particles and objects of particles

    //Particles
    for (auto pT: simulationSettings.particles) {
        Particle p{pT.x, pT.v, pT.m};
        model->addParticle(p);
    }
    //Cuboids
    for (auto cuboid: simulationSettings.cuboids) {
        model->addCuboid(cuboid.position, cuboid.dimensions[0], cuboid.dimensions[1], cuboid.dimensions[2], cuboid.h,
                         cuboid.mass, cuboid.initVelocity, cuboid.dimensionsBrownianMotion,
                         cuboid.brownianMotionAverageVelocity);
    }
    //Discs
    for (auto disc: simulationSettings.discs) {
        model->addDisc(disc.center, disc.initVelocity, disc.N, disc.h, disc.mass, disc.dimensionsBrownianMotion,
                       disc.brownianMotionAverageVelocity);
    }
}

Simulator::Simulator(DirectSumSimulationParameters &parameters, std::string &inputFilePath,
                     FileHandler::inputFormat inputFormat, FileHandler::outputFormat outputFormat,
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
            force = std::make_unique<LeonardJonesForce>(parameters.epsilon, parameters.sigma);
        }
        break;
        default: {
            spdlog::error("Use of invalid force type! Programm will be terminated!");
            exit(-1);
        }
    }
    model = std::make_unique<DirectSum>(*force, parameters.deltaT, inputFormat, outputFormat);
    model->addViaFile(inputFilePath);
}

void Simulator::run(bool benchmark) {
    double current_time = 0;
    int iteration = 0;

    //Plot everything one time before the simulation starts.
    if (!benchmark) {
        model->plot(iteration, outputFileBaseName);
    }

    //Calculate the initial forces before starting the simulation
    model->updateForces();


    while (current_time < endT) {
        model->step();
        iteration++;
        if (!benchmark && iteration % outputFrequency == 0) {
            model->plot(iteration, outputFileBaseName);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

ParticleContainer& Simulator::getParticles() {
    return model->getParticles();
}
