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
                     FileHandler::inputFormat inputFormat,
                     FileHandler::outputFormat outputFormat) : deltaT{parameters.deltaT}, endT{parameters.endT} {
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

Simulator::Simulator(LinkedCellsSimulationParameters &parameters, std::string &inputFilePath,
                     FileHandler::inputFormat inputFormat,
                     FileHandler::outputFormat outputFormat) : deltaT{parameters.deltaT}, endT{parameters.endT} {
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
                                          parameters.sigma, inputFormat, outputFormat, parameters.boundrySettings);
    model->addViaFile(inputFilePath);
}

void Simulator::run(bool benchmark) {
    double current_time = 0;

    int iteration = 0;

    //Calculate the initial forces before starting the simulation
    model->updateForces();


    while (current_time < endT) {
        model->step();
        iteration++;
        if (!benchmark && iteration % 100 == 0) {
            model->plot(iteration);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

ParticleContainer &Simulator::getParticles() {
    return model->getParticles();
}
