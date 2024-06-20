//
// Created by daniel on 04.05.24.
//

#pragma once
#include "../Force.h"

class LeonardJonesForce : public Force {
private:

    const double epsilon;
    const double sigma;

public:
    /**
     * @brief Initialize leonardJonesForce calculation with required constants.
     *
     * @param epsilon Constant epsilon.
     * @param sigma Constant sigma.
     */
    LeonardJonesForce(double epsilon, double sigma);

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
};
