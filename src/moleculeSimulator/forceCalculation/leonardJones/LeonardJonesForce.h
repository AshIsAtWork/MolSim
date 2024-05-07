//
// Created by daniel on 04.05.24.
//

#pragma once
#include "../Force.h"

class LeonardJonesForce : public Force{
private:

    //Hard encoded constants for computation of the LeonardJonesForce. May be non const in future implementations

    static double const epsilon;
    static double const sigma;
public:
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
