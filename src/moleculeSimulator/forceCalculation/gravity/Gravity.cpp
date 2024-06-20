//
// Created by daniel on 24.04.24.
//
#include "Gravity.h"

std::array<double, 3> Gravity::compute(Particle &target, Particle &source) {
    return (target.getM() * source.getM()) / std::pow(ArrayUtils::L2Norm(target.getX() - source.getX()), 3.0) *
           (source.getX() - target.getX());
}

std::array<double, 3> Gravity::computeOptimized(Particle &target, Particle &source, std::array<double, 3> &difference,
    double distance) {
    spdlog::error("Not implemented yet!");
    exit(-1);
}
