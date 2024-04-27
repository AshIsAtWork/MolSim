//
// Created by daniel on 24.04.24.
//

#include "Gravity.h"

#include "utils/ArrayUtils.h"

const std::array<double, 3> Gravity::compute(Particle &target, Particle &source) {
    double distance = ArrayUtils::L2Norm(target.getX() - source.getX());
    double scalar = (target.getM() * source.getM()) / std::pow(distance,3.0);
    return scalar * (source.getX() - target.getX());
}

