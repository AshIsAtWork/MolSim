//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"

#include <memory>

#include "forceCalculation/gravity/Gravity.h"
#include "forceCalculation/leonardJones/LeonardJonesForce.h"
#include "models/DirectSum.h"
//TODO: Enable this for below optimization
//#include <ranges>

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
    //model->addViaFile(inputFilePath);
    model->addDisc({60, 25, 0}, {0, -10, 0}, 15, 1.1225, 1, 2, 0.1);
}

Simulator::Simulator(LinkedCellsSimulationParameters &parameters, std::string &inputFilePath,
                     FileHandler::inputFormat inputFormat,
                     FileHandler::outputFormat outputFormat, int outputFrequency,
                     std::string &outputFileBaseName) : deltaT{parameters.deltaT},
                                              endT{parameters.endT}, outputFrequency{outputFrequency},
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
    model = std::make_unique<LinkedCells>(*force, parameters.deltaT, parameters.domainSize, parameters.rCutOff,
                                          parameters.sigma, inputFormat, outputFormat, parameters.boundarySettings);
    //model->addViaFile(inputFilePath);
    model->addDisc({60, 25, 0.5}, {0, -10, 0}, 15, 1.1225, 1, 3, 0.1);
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

ParticleContainer &Simulator::getParticles() {
    return model->getParticles();
}
