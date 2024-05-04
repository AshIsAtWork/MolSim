//
// Created by daniel on 04.05.24.
//

#pragma once
#include "Force.h"

class LeonardJonesForce : public Force{
private:
    static double const epsilon;
    static double const sigma;
public:
    std::array<double, 3> compute(Particle &target, Particle &source) override;
};
