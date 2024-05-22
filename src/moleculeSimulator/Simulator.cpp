//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"
//TODO: Enable this for below optimization
//#include <ranges>

void Simulator::calculateF_naive() {
    for (auto &p_i: particles) {
        p_i.setOldF(p_i.getF());
        p_i.setF({0, 0, 0});
        for (auto &p_j: particles) {
            if (&p_i != &p_j) {
                p_i.setF(p_i.getF() + force.compute(p_i, p_j));
            }
        }
    }
}

void Simulator::calculateF() {
    //Reset all forces and save current forces in old force variables.
    for (auto &p: particles) {
        p.setOldF(p.getF());
        p.setF({0, 0, 0});
    }

    //Iterate over all distinct pairs of particles and apply Newtons third law of motion.

    for (auto p_i = particles.begin(); p_i != particles.end(); std::advance(p_i, 1)) {
        for (auto p_j = std::next(p_i); p_j != particles.end(); std::advance(p_j, 1)) {
            auto f_ij{force.compute(*p_i, *p_j)};
            p_i->setF(p_i->getF() + f_ij);
            p_j->setF(p_j->getF() - f_ij);
        }
    }

//    TODO: Possible Improvement (BUT USES C++23) Change it in CMAKE to test it
//    for (auto& p : particles) {
//        p.setOldF(p.getF());
//        p.setF({0, 0, 0});
//    }
//
//// Iterate over all distinct pairs of particles and apply Newton's third law of motion
//    for (const auto& [p_i, p_j] : std::views::cartesian_product(particles, particles)
//                                  | std::views::filter([](const auto& pair) { return &pair.first != &pair.second; })) {
//        auto f_ij = force.compute(p_i, p_j);
//        p_i.setF(p_i.getF() + f_ij);
//        p_j.setF(p_j.getF() - f_ij);
//    }
}

void Simulator::calculateX() {
    for (auto &p: particles) {
        p.setX(p.getX() + deltaT * p.getV() + ((deltaT * deltaT) / (2.0 * p.getM())) * p.getOldF());
    }
}

void Simulator::calculateV() {
    for (auto &p: particles) {
        p.setV(p.getV() + (deltaT / (2 * p.getM())) * (p.getOldF() + p.getF()));
    }
}

Simulator::Simulator(std::string &inputFilePath, Force &force, double endT, double deltaT,
                     FileHandler::inputFormat inputFormat, FileHandler::outputFormat outputFormat) : force{force},
                     deltaT{deltaT}, endT{endT}, inputFormat{inputFormat}, outputFormat{outputFormat} {
    FileHandler::readFile(particles, inputFilePath, inputFormat);
}

void Simulator::run(bool benchmark) {
    double current_time = 0;

    int iteration = 0;

    //Calculate the initial forces before starting the simulation
    calculateF();

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endT) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (!benchmark && iteration % 10 == 0) {
            fileHandler.writeToFile(particles, iteration, outputFormat);
        }

        spdlog::trace("Iteration {} finished.", iteration);

        current_time += deltaT;
    }

    spdlog::info("Output written. Terminating...");
}

DefaultParticleContainer& Simulator::getParticles() {
    return particles;
}
