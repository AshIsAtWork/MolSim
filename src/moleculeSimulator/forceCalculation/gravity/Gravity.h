//
// Created by daniel on 24.04.24.
//

#pragma once

#include "moleculeSimulator/forceCalculation/Force.h"

#include "utils/ArrayUtils.h"


/**
 * @brief Implementation of the gravitational force.
 *
 * The gravitational force is one example of a force which might act between two particles in space.
 */
class Gravity : public Force {
public:
    /**
    * @brief Actual computation of the gravitational force occurring.
    *
    * @param target Particle on which the gravitational force acts.
    * @param source Particle which exerts the gravitational force on the target.
    * @return 3 dimensional force vector.
    *
    * Computation of the gravitational force which exerts the source on the target.
    */
    std::array<double, 3> compute(Particle &target, Particle &source) override;

    /**
    * @brief We needed this to integrate our optimization in our code to prevent breaking the inheritance hierachy.
    *        Will be removed, when we integrated the optimization properly.
    */

    std::array<double, 3> computeOptimized(Particle &target, Particle &source, std::array<double, 3>& difference, double distance) override;
};
