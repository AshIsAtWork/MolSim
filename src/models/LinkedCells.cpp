//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, FileHandler::inputFormat inputFormat,
                         FileHandler::outputFormat outputFormat) : Model(particles, force, deltaT, inputFormat,
                                                                         outputFormat), particles(domainSize, rCutOff) {
}

void LinkedCells::step() {
    updatePositions();
    updateForces();
    updateVelocities();
    particles.updateCells();
}
