//
// Created by daniel on 22.05.24.
//
#pragma once

#include "Model.h"
#include "particleRepresentation/container/LinkedCellsContainer.h"

/**
 * @brief Model that implements the linked cell algorithm
 */

class LinkedCells final : public Model {
public:
    /**
     * Enum to distinguish between different boundry conditions for each side.
     */
    enum class BoundryCondition { outflow, reflective };

private:
    /**
     * This model uses the LinkedCellsContainer to store its particles
     */
    LinkedCellsContainer particles;
    /**
     * Definition of the boundry condition for each side.
     */
    std::array<std::pair<LinkedCellsContainer::Side, BoundryCondition>, 6> boundrySettings;

    /**
     * Threshhold for the distance that specifies, when ghost particles are spawned.
     */
    double threshhold;

    /**
     * @brief Helper method that handles the boundry conditions
     */
    void processBoundries();

public:
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff, double sigma,
                FileHandler::inputFormat inputFormat,
                FileHandler::outputFormat outputFormat,
                std::array<std::pair<LinkedCellsContainer::Side, BoundryCondition>, 6> &boundrySettings);

    /**
     * @brief Perform one time step in the linked cells model.
     */
    void step() override;
};
