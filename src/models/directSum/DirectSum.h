//
// Created by daniel on 22.05.24.
//

#pragma once

#include "../Model.h"
#include "particleRepresentation/container/DefaultParticleContainer.h"

/**
 * @brief Model that implements the direct sum algorithm
 */
class DirectSum final : public Model {
private:
    /**
    * This model uses the DefaultParticleContainer to store its particles
    */
    DefaultParticleContainer particles;

public:
    DirectSum(Force &force, double deltaT, FileHandler::inputFormat inputFormat,
              FileHandler::outputFormat outputFormat);

    /**
     * @brief Perform one time step in the direct sum model.
     */
    void step() override;
};
