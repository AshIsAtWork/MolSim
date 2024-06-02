//
// Created by daniel on 22.05.24.
//
#pragma once

#include "../Model.h"
#include "../../particleRepresentation/container/LinkedCellsContainer.h"

/**
 * @brief Model that implements the linked cell algorithm
 *
 * @details The linked-cells algorithm is significantly faster than the direct sum algorithm. It reduces the time complexity
 *          from O(n²) to O(n). The following plot shows some benchmarks.
 * @image html Benchmarks.png "Benchmarks" width=600px
 */

class LinkedCells final : public Model {
private:
    /**
     * This model uses the LinkedCellsContainer to store its particles
     */
    LinkedCellsContainer particles;
    /**
     * Definition of the boundary condition for each side.
     */
    std::array<std::pair<Side,enumsStructs::BoundaryCondition>, 6> boundarySettings;

    /**
     * Threshold for the distance that specifies, when ghost particles are spawned.
     */
    double threshold;

    /**
     * @brief Helper method that handles the boundry conditions
     */
    void processBoundaries();

public:
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff, double sigma,
                FileHandler::inputFormat inputFormat,
                FileHandler::outputFormat outputFormat,
                std::array<std::pair<Side, enumsStructs::BoundaryCondition>, 6> &boundarySettings);

    /**
     * @brief Perform one time step in the linked cells model.
     */
    void step() override;
};
