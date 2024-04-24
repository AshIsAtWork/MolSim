//
// Created by daniel on 24.04.24.
//

#pragma once
#include "particleRepresentation/Particle.h"

class Force {
public:
    virtual const std::array<double, 3> compute(Particle& p1, Particle& p2) = 0;
    virtual ~Force() = default;
};
