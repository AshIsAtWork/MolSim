//
// Created by daniel on 04.05.24.
//

#include "LeonardJonesForce.h"

double const LeonardJonesForce::sigma = 1;
double const LeonardJonesForce::epsilon = 5;

std::array<double, 3> LeonardJonesForce::compute(Particle &target, Particle &source) {
    double distance = ArrayUtils::L2Norm(target.getX() - source.getX());
    double c1 = pow(sigma / distance, 6);
    double c2 = 2 * c1 * c1;
    return -(24 * epsilon / distance * distance) * (c1 - c2) * (target.getX() - source.getX());
}

