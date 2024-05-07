//
// Created by daniel on 04.05.24.
//

#include "LeonardJonesForce.h"

double const LeonardJonesForce::sigma = 1;
double const LeonardJonesForce::epsilon = 5;


std::array<double, 3> LeonardJonesForce::compute(Particle &target, Particle &source) {
    auto difference = source.getX() - target.getX();
    double squared_distance = std::pow(ArrayUtils::L2Norm(difference), 2);
    double c1 = std::pow(sigma * sigma / squared_distance, 3);
    double c2 = 2 * c1 * c1;
    return ((24 * epsilon) / squared_distance) * (c1 - c2) * difference;
}
