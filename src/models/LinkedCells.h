//
// Created by daniel on 22.05.24.
//
#pragma once

#include "Model.h"
#include "particleRepresentation/container/LinkedCellsContainer.h"

class LinkedCells final : public Model {
private:
    LinkedCellsContainer particles;

public:
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff, FileHandler::inputFormat inputFormat,
    FileHandler::outputFormat outputFormat);

    void step() override;
};
