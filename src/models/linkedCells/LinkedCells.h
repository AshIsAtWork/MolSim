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
    enumsStructs::ParallelizationStrategy parallelizationStrategy;

    /**
     * This model uses the LinkedCellsContainer to store its particles
     */
    LinkedCellsContainer particles;
    /**
     * Definition of the boundary condition for each side.
     */
    std::vector<std::pair<Side, enumsStructs::BoundaryCondition> > boundarySettings;

    /**
     * @brief Apply forces to all particles in boundary cells according to the specified boundary conditions.
     */
    void processBoundaryForces();

    /**
     * @brief Process all particles which have left the domain at the end of a simulation step according
     *        to the specified boundary conditions.
     */
    void processHaloCells();

    /**
     * @brief Apply to all particles that are marked a constant force.
     */
    void pullMarkedParticles();

    /**
     * @brief Calculate forces between particles in a membrane structure.
     */
    void updateForcesMembrane();

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
     * @param parallelizationStrategy Strategy to use for parallelization.
     */
    LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize, double rCutOff,
                FileHandler::outputFormat outputFormat, BoundarySet boundaryConditions, bool gravityOn,
                std::array<double, 3> g = {}, MembraneParameters membraneParameters = MembraneParameters{},
                enumsStructs::ParallelizationStrategy parallelizationStrategy =
                        enumsStructs::ParallelizationStrategy::none);

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

    /**
     * @brief Calculate forces at the beginning of the simulation that the old force is not 0.
     */
    void initializeForces() override;

 //The following methods are only needed for parallelization.

#ifdef _OPENMP
    /**
     * @brief Implements parallel force calculation by using reduction.
     */
    void updateForcesParallelReduction();

    /**
     * @brief Implements parallel force calculation by using the skipping schedule.
     */
    void updateForcesParallelSkipping();

    /**
     * @brief Implements parallel force calculation by using the linear schedule.
     */
    void updateForcesParallelLinear();

    /**
     * @brief Implements parallel calculation of velocity updates.
     */
    void updateVelocitiesParallel();

    /**
     * @brief Implements parallel calculation of position updates.
     */
    void updatePositionsParallel();

    /**
     * @brief Implements parallel calculation of force updates at boundaries.
     */
    void processBoundaryForcesParallel();

    /**
     * @brief Prepares reduction vectors for parallelization strategy "Reduction"
     *
     * @param maxNumThreads Maximum number of threads that is used during the reduction.
     */
    void initializeReductionVectors(int maxNumThreads);
#endif
};
