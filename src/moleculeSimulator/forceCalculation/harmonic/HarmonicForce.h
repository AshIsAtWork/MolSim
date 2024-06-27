//
// Created by daniel on 27.06.24.
//

#pragma once
#include "moleculeSimulator/forceCalculation/Force.h"


class HarmonicForce : public Force {
private:
    double k;
    double r0;

public:
    /**
     * @brief Initialise the harmonic force. This force acts between neighbors in a membrane.
     *
     * @param k Stiffness constant.
     * @param r0 Average bond length.
     */
    HarmonicForce(double k, double r0);

    /**
    * @brief Actual computation of the harmonic force occurring.
    *
    * @param target Particle on which the harmonic force acts.
    * @param source Particle which exerts the harmonic force on the target.
    * @return 3 dimensional force vector.
    *
    * Computation of the harmonic force which exerts the source on the target.
    */
    std::array<double, 3> compute(Particle &target, Particle &source) override;

    std::array<double, 3> computeOptimized(Particle &target, Particle &source, std::array<double, 3> &difference,
    double distance) override;
};
