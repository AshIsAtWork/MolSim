//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"

#include "particleGeneration/ParticleGenerator.h"

void Simulator::calculateF_naive() {
    for (auto &p_i : particles) {
        p_i.setOldF(p_i.getF());
        p_i.setF({0,0,0});
        for (auto &p_j : particles) {
            if(&p_i != &p_j) {
                p_i.setF(p_i.getF() + force.compute(p_i, p_j));
            }
        }
    }
}

void Simulator::calculateF() {

    for(auto &p : particles) {
        p.setOldF(p.getF());
        p.setF({0,0,0});
    }

    //Iterate over all distinct pairs of particles and apply Newtons third law of motion.

    for(auto p_i = particles.begin(); p_i != particles.end(); ++p_i) {
        for(auto p_j = p_i + 1; p_j != particles.end(); ++p_j) {
            auto f_ij{force.compute(*p_i, *p_j)};
            p_i->setF(p_i->getF() + f_ij);
            p_j->setF(p_j->getF() - f_ij);
        }
    }
}

void Simulator::calculateX() {
    for (auto &p : particles) {
        p.setX(p.getX() + deltaT * p.getV() + ((deltaT * deltaT) / (2.0 * p.getM())) * p.getOldF());
    }
}

void Simulator::calculateV() {
    for (auto &p : particles) {
        p.setV(p.getV() + (deltaT / (2 * p.getM())) * (p.getOldF() + p.getF()));
    }
}

Simulator::Simulator(std::string &inputFilePath, Force &force, double endT, double deltaT) : force{force},
    deltaT{deltaT}, endT{endT} {
    //FileHandler::readFile(particles, inputFilePath);
    ParticleGenerator::generateCuboid(particles, {0,0,0}, 40,8,1,1.1225,1,{0,0,0});
    ParticleGenerator::generateCuboid(particles, {15,15,0}, 8,8,1,1.1125,1,{0,-10,0});
}

void Simulator::run() {

    double current_time = 0;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endT) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            fileHandler.writeToFile(particles, iteration,FileHandler::outputFormat::vtk);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += deltaT;
    }

    std::cout << "output written. Terminating..." << std::endl;
}
