//
// Created by daniel on 22.05.24.
//

#include "DirectSum.h"

DirectSum::DirectSum(Force &force, double deltaT, FileHandler::outputFormat outputFormat) : Model(
    particles, force, deltaT, outputFormat) {
}

void DirectSum::step() {
    updateForces();
    updateVelocities();
    updatePositions();
}
