//
// Created by daniel on 22.05.24.
//

#pragma once

#include "../Model.h"
#include "particleRepresentation/container/defaultParticleContainer/DefaultParticleContainer.h"

/**
 * @brief Model that implements the direct sum algorithm.
 */
class DirectSum final : public Model {
private:
    /**
    * This model uses the DefaultParticleContainer to store its particles.
    */
    DefaultParticleContainer particles;

public:
    /**
     * @brief Construct a new Direct Sum model.
     *
     * @param force Force to use.
     * @param deltaT Discretisation step.
     * @param outputFormat Output format.
     * @param gravityOn Toggle gravity on or off.
     * @param g Gravitational factor g.
     */
    DirectSum(Force &force, double deltaT, FileHandler::outputFormat outputFormat, bool gravityOn, double g = 1);

    /**
     * @brief Perform one time step in the direct sum model.
     */
    void step() override;
};
