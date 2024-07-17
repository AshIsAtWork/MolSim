//
// Created by daniel on 22.05.24.
//

#include "DirectSum.h"

DirectSum::DirectSum(Force &force, double deltaT, FileHandler::outputFormat outputFormat, bool gravityOn, double g) : Model(
    particles, force, deltaT, outputFormat, gravityOn, g) {
}

void DirectSum::step() {
    updateForces();
    if(gravityOn) {
        applyGravity();
    }
    updateVelocities();
    updatePositions();
}
