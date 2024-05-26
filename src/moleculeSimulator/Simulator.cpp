//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"
//TODO: Enable this for below optimization
//#include <ranges>

Simulator::Simulator(std::string &inputFilePath, Model &model, Force &force, double endT, double deltaT,
                     FileHandler::inputFormat inputFormat, FileHandler::outputFormat outputFormat) : model{model},
    force{force},
    deltaT{deltaT}, endT{endT} {
    model.addDisc({60, 25, 4.5},{0,-10,0},15,1.1225,1,3);
    //model.addViaFile(inputFilePath);
}

void Simulator::run(bool benchmark) {
    double current_time = 0;

    int iteration = 0;

    //Calculate the initial forces before starting the simulation
    model.updateForces();


    while (current_time < endT) {
        model.step();
        iteration++;
        if (!benchmark && iteration % 100 == 0) {
            model.plot(iteration);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

ParticleContainer& Simulator::getParticles() {
    return model.getParticles();
}
