//
// Created by daniel on 04.05.24.
//

#pragma once
#include "../Force.h"

class LeonardJonesForce : public Force {

public:

    LeonardJonesForce();

    /**
    * @brief Actual computation of the Leonard-Jones force occurring.
    *
    * @param target Particle on which the Leonard-Jones force acts.
    * @param source Particle which exerts the Leonard-Jones force on the target.
    * @return 3 dimensional force vector.
    *
    * Computation of the Leonard-Jones force which exerts the source on the target.
    */
    std::array<double, 3> compute(Particle &target, Particle &source) override;

    /**
    * @brief Implements the optimization we presented as our second idea.
    *        At the moment this is dead code, because we have not been able yet to make it compatible
    *        with our current program structure and only integrated it once when doing the time measurements.
    */

    std::array<double, 3> computeOptimized(Particle &target, Particle &source, std::array<double, 3>& difference, double distance) override;
};
