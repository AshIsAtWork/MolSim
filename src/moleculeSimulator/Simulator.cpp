//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"
#include "utils/ArrayUtils.h"

#include <iostream>

#include "ForceCalculation/Gravity.h"

void Simulator::calculateF() {
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
    FileHandler::readFile(particles, inputFilePath);
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
