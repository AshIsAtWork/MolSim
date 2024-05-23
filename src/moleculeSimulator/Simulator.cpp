//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"
//TODO: Enable this for below optimization
//#include <ranges>

Simulator::Simulator(std::string &inputFilePath, Model& model, Force &force, double endT, double deltaT) : model{model},force{force},
    deltaT{deltaT}, endT{endT} {
    model.addViaFile(inputFilePath);
}

void Simulator::run(bool benchmark) {
    double current_time = 0;

    int iteration = 0;

    //Calculate the initial forces before starting the simulation
    model.updateForces();


    while (current_time < endT) {
        model.step();
        iteration++;
        if (!benchmark && iteration % 10 == 0) {
            model.plot(iteration, FileHandler::outputFormat::vtk);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

ParticleContainer& Simulator::getParticles() {
    return model.getParticles();
}
