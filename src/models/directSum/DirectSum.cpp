//
// Created by daniel on 22.05.24.
//

#include "DirectSum.h"

DirectSum::DirectSum(Force &force, double deltaT, FileHandler::inputFormat inputFormat,
    FileHandler::outputFormat outputFormat) : Model(particles, force, deltaT, inputFormat, outputFormat) {
}

void DirectSum::step() {
    updateForces();
    updateVelocities();
    updatePositions();
}
