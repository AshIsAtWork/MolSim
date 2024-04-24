//
// Created by daniel on 24.04.24.
//

#pragma once
#include "Force.h"

class Gravity : public Force{
public:
    const std::array<double, 3> compute(Particle &target, Particle &source) override;
};
