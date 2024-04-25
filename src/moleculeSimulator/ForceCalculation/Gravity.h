//
// Created by daniel on 24.04.24.
//

#pragma once
#include "Force.h"

/**
 * @brief Implementation of the gravitational force.
 *
 * The gravitational force is one example of a force which might act between two particles in space.
 */
class Gravity : public Force{
public:
    /**
    * @brief Actual computation of the gravitational force occuring.
    *
    * @param target Particle on which the gravitational force acts.
    * @param source Particle which exerts the gravitational force on the target.
    * @return 3 dimensional force vector.
    *
    * Computation of the gravitational force which exerts the source on the target.
    */
    const std::array<double, 3> compute(Particle &target, Particle &source) override;
};
