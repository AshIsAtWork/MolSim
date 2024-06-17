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

    /**
     * @brief Apply forces to all particles in boundary cells according to the specified boundary conditions.
     */
    void processBoundaryForces();

    /**
     * @brief Process all particles which have left the domain at the end of a simulation step according
     *        to the specified boundary conditions.
     */
    void processHaloCells();

public:
    /**
     * @brief Contruct a new Linked Cells model.
     *
     * @param force Force to use.
     * @param deltaT Discretisation step.
     * @param domainSize Domain size.
     * @param rCutOff Cutoff radius.
     * @param outputFormat Output format.
     * @param boundaryConditions Boundary conditions.
     * @param gravityOn Toggle gravity on or off.
     * @param g Gravitational factor.
     */
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff,
                FileHandler::outputFormat outputFormat, BoundarySet boundaryConditions, bool gravityOn, double g = 1);

    /**
     * @brief Perform one time step in the linked cells model.
     */
    void step() override;
};
