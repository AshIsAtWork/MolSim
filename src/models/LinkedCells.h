//
// Created by daniel on 22.05.24.
//
#pragma once

#include "Model.h"
#include "particleRepresentation/container/LinkedCellsContainer.h"

class LinkedCells final : public Model {
public:
    enum class BoundryCondition {outflow, reflective};

private:
    LinkedCellsContainer particles;
    BoundryCondition boundryCondition;
    std::array<LinkedCellsContainer::Boundry, 6> boundries = {
        LinkedCellsContainer::Boundry::front,
        LinkedCellsContainer::Boundry::back,
        LinkedCellsContainer::Boundry::left,
        LinkedCellsContainer::Boundry::right,
        LinkedCellsContainer::Boundry::top,
        LinkedCellsContainer::Boundry::bottom
    };

    //Threshhold for the distance that specifies, when ghost particles are spawned.
    static constexpr double threshhold = pow(2.0, 1.0 / 6.0);

public:
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff, FileHandler::inputFormat inputFormat,
    FileHandler::outputFormat outputFormat, BoundryCondition boundryCondition);

    void processBoundries();
    void step() override;
};
