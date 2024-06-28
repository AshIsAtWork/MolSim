//
// Created by daniel on 22.05.24.
//
#pragma once

#include "../Model.h"
#include "../../particleRepresentation/container/linkedCellsContainer/LinkedCellsContainer.h"
#include "moleculeSimulator/forceCalculation/harmonic/HarmonicForce.h"

/**
 * @brief Model that implements the linked cell algorithm
 *
 * @details The linked-cells algorithm is significantly faster than the direct sum algorithm. It reduces the time complexity
 *          from O(n²) to O(n). The following plot shows some benchmarks.
 * @image html Benchmarks.png "Benchmarks" width=600px
 */

class LinkedCells final : public Model {
private:
    bool membraneSetting;
    bool pull;
    int pullingActiveUntil;
    std::array<double, 3> pullingForce{};
    std::unique_ptr<Force> forceBetweenDiagonalNeighborsInMembrane;
    std::unique_ptr<Force> forceBetweenDirectNeighborsInMembrane;

    /**
     * This model uses the LinkedCellsContainer to store its particles
     */
    LinkedCellsContainer particles;
    /**
     * Definition of the boundary condition for each side.
     */
    std::vector<std::pair<Side, enumsStructs::BoundaryCondition>> boundarySettings;

    std::vector<std::shared_ptr<Particle>> particlesToPull;

    /**
     * @brief Apply forces to all particles in boundary cells according to the specified boundary conditions.
     */
    void processBoundaryForces();

    /**
     * @brief Process all particles which have left the domain at the end of a simulation step according
     *        to the specified boundary conditions.
     */
    void processHaloCells();

    void pullSelectedParticles();

    void applyForcesBetweenNeighborsInMembrane();

    void updateForcesTruncated();

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
     * @param membraneParameters Contains specification if a membrane is simulated
     */
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff,
                FileHandler::outputFormat outputFormat, BoundarySet boundaryConditions, bool gravityOn, std::array<double, 3> g = {}, MembraneParameters membraneParameters = MembraneParameters{});

    /**
     * @brief Perform one time step in the linked cells model.
     *
     * @param iteration Current iteration the simulator is in.
     */
    void step(int iteration) override;

    /**
     * @brief Implements the optimization we presented as our second idea.
     *        At the moment this is dead code, because we did not have time yet to make it compatible
     *        with our current program structure and only integrated it once when doing the time measurements.
     */
    void updateForcesOptimized();

    void initializeForces() override;
};
