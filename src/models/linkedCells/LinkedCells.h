//
// Created by daniel on 22.05.24.
//
#pragma once

#include "../Model.h"
#include "../../particleRepresentation/container/linkedCellsContainer/LinkedCellsContainer.h"

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
    std::vector<std::pair<Side,enumsStructs::BoundaryCondition>> boundarySettings;

    void processBoundaryForces();

    void processHaloCells();

public:
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff,
                FileHandler::outputFormat outputFormat, BoundarySet boundaryConditions, bool gravityOn, double g);

    /**
     * @brief Perform one time step in the linked cells model.
     */
    void step() override;
};
