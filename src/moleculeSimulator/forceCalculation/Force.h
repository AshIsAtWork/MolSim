//
// Created by daniel on 24.04.24.
//

#pragma once
#include "particleRepresentation/particle/Particle.h"

/**
 * @brief Interface representing the force that the source exerts on the target.
 *
 * This interface provides a generalization of the force that might occur between two particles.
 * There are different types of forces which may all implement this interface.
 */
class Force {
public:
    /**
     * @brief Actual computation of the force occurring.
     *
     * @param target Particle on which the force acts.
     * @param source Particle which exerts the force on the target.
     * @return 3 dimensional force vector.
     *
     * This method may be implemented by any child of this class according to the type of force it represents.
     */
    virtual std::array<double, 3> compute(Particle &target, Particle &source) = 0;

    virtual ~Force() = default;

    /**
    * @brief We needed this to integrate our optimization in our code to prevent breaking the inheritance hierachy.
    *        Will be removed, when we integrated the optimization properly.
    */

    virtual std::array<double, 3> computeOptimized(Particle &target, Particle &source, std::array<double, 3>& difference, double distance) = 0;
};
