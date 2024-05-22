//
// Created by daniel on 22.05.24.
//

#include "DirectSum.h"

DirectSum::DirectSum(Force &force, double deltaT) : Model(particles, force, deltaT) {
}

void DirectSum::step() {
    updatePositions();
    updateForces();
    updateVelocities();
}
