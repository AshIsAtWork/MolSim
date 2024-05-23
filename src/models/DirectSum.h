//
// Created by daniel on 22.05.24.
//

#pragma once

#include "Model.h"
#include "particleRepresentation/container/DefaultParticleContainer.h"

class DirectSum final : public Model {
private:
    DefaultParticleContainer particles;

public:
    DirectSum(Force &force, double deltaT, FileHandler::inputFormat inputFormat,
              FileHandler::outputFormat outputFormat);

    void step() override;
};
