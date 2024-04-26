//
// Created by daniel on 24.04.24.
//

#include "Simulator.h"
#include "utils/ArrayUtils.h"
#include "fileHandling/outputWriter/VTKWriter.h"
#include "particleRepresentation/ParticleContainer.h"

#include <iostream>

#include "ForceCalculation/Gravity.h"

void Simulator::calculateF() {
    for (auto &p_i : particles) {
        p_i.setOldF(p_i.getF());
        p_i.setF({0,0,0});
        for (auto &p_j : particles) {
            if(&p_i != &p_j) {
                p_i.setF(p_i.getF() + gravity.compute(p_i, p_j));
            }
        }
    }
}

void Simulator::calculateX() {
    for (auto &p : particles) {
        std::array<double, 3> xNew = p.getX() + deltaT * p.getV() + ((deltaT * deltaT) / (2.0 * p.getM())) * p.getOldF();
        p.setX(xNew);
    }
}

void Simulator::calculateV() {
    for (auto &p : particles) {
        std::array<double, 3> vNew = p.getV() + (deltaT / (2 * p.getM())) * (p.getOldF() + p.getF());
        p.setV(vNew);
    }
}


Simulator::Simulator(std::string& inputFilePath) {
    fileHandler.readFile(particles, inputFilePath);
    deltaT = 0.014;
    endT = 1000;
}

void Simulator::configure(double endT, double deltaT) {
    this->endT = endT;
    this->deltaT = deltaT;
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

ParticleContainer Simulator::getParticles() {
    return particles;
}
