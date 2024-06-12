//
// Created by daniel on 04.05.24.
//

#include "LeonardJonesForce.h"


LeonardJonesForce::LeonardJonesForce()= default;

std::array<double, 3> LeonardJonesForce::compute(Particle &target, Particle &source) {
    //compute mixing constants
    double sigma_ij = (target.getSigma() + source.getSigma()) / 2;
    double epsilon_ij = std::sqrt(target.getEpsilon() * source.getEpsilon());
    auto difference = source.getX() - target.getX();
    double squared_distance = std::pow(ArrayUtils::L2Norm(difference), 2);
    double c1 = std::pow(sigma_ij * sigma_ij / squared_distance, 3);
    double c2 = 2 * c1 * c1;
    return ((24 * epsilon_ij) / squared_distance) * (c1 - c2) * difference;
}
