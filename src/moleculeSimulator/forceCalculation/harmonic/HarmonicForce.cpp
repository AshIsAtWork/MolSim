//
// Created by daniel on 27.06.24.
//

#include "HarmonicForce.h"

HarmonicForce::HarmonicForce(double k, double r0) : k{k}, r0{r0}{}

std::array<double, 3> HarmonicForce::compute(Particle &target, Particle &source) {
    auto difference = source.getX() - target.getX();
    double distance = ArrayUtils::L2Norm(difference);
    return ((k * (distance - r0)) / distance) * difference;
};

std::array<double, 3> HarmonicForce::computeOptimized(Particle &target, Particle &source, std::array<double, 3> &difference,
    double distance) {
    spdlog::error("Not implemented yet!");
    exit(-1);
}


