//
// Created by daniel on 21.05.24.
//

#pragma once
#include <vector>

#include "../ParticleContainer.h"
#include "fileHandling/outputWriter/VTKWriter/VTKWriter.h"
#include "particleRepresentation/particle/Particle.h"
#include "utils/enumsStructs.h"
#include "moleculeSimulator/forceCalculation/leonardJones/LeonardJonesForce.h"

using namespace enumsStructs;

/**
 * @brief Container to store the particles for simulation using the linked cells algorithm.
 */
class LinkedCellsContainer : public ParticleContainer{

private:

    //Data structure

    /**
     * We use an 1D vector to store the flattened 3D cell structure
     * being an essential property of the linked cells algorithm. Each cell is represented itself by an 1D vector of particles.
     */
    std::vector<std::vector<Particle>> cells;

    /**
     * The current number of particles that is contained in this container is tracked by the attribute currentSize and kept up-to-date
     * through every operation.
     */
    size_t currentSize;


     //Precomputed indices:
     //Most of the indices are precomputed in the constructor and stored in memory to provide fast access in future iterations.
     //We invest here additional memory space to avoid recomputing all indices in each iteration again.

    /**
     * The indices of all halo cells belonging to one or more of the 6 sides,
     */
    std::array<std::vector<int>, 6> haloCells;
    /**
     * The indices of all boundary cells belonging to one or more of the 6 sides
     */
    std::array<std::vector<int>, 6> boundaries;
    /**
     * The indices of the order in which all cells within the domain are processed to process each pair of particles only ones
     */
    std::vector<std::vector<int>> domainCellIterationScheme;


    //All parameters used in this model:

    /**
     * Number of cells in dimension x (halo cells inclusive).
     */
    int nX;
    /**
     * Number of cells in dimension y (halo cells inclusive).
     */
    int nY;
    /**
     * Number of cells in dimension z (halo cells inclusive).
     */
    int nZ;
    /**
     * Number of cells in one row (nX many).
     */
    int baseY;
    /**
     * Number of cells in one layer (nX * nY many).
     */
    int baseZ;
    /**
     * Size of each cell in dimension x.
     */
    double cellSizeX;
    /**
     * Size of each cell in dimension y.
     */
    double cellSizeY;
    /**
     * Size of each cell in dimension z.
     */
    double cellSizeZ;
    /**
     * Cut-off radius.
     */
    double rCutOff;
    /**
     * Specifies, if the simulation only uses 2 of 3 dimensions. If this is the case, resources can be saved.
     */
    bool twoD;

    /**
     * Size of the domain {x, y , z}. The front lower left corner of the domain is set to (0,0,0) by definition.
     */
    std::array<double, 3> domainSize;

    outputWriter::VTKWriter vtk_writer;

    LeonardJonesForce lJF;

    BoundarySet boundariesSet;


    //Helper methods for index calculation

    /**
     * @brief Pre-calculation of all halo cell indices.
     */
    void calculateHaloCellIndices();

    /**
     * @brief Pre-calculation of all boundary cell indices.
     */
    void calculateBoundaryCellIndices();

    /**
     * @brief Pre-calculation of the indices defining the processing order of cells.
     */
    void calculateDomainCellsIterationScheme();

    /**
     * @brief Calculate the distance that a particle has to a specific side of the domain.
     *        In 2D the distance to sides top and bottom is always 0.
     *
     * @param p Particle used for distance calculation
     * @param side Side of the domain to which the distance is calculated
     *
     * @return Distance of particle p to the specified side.
     */
    double calcDistanceFromBoundary(Particle& p, Side side);

    /**
     * @brief Calculate the position of the ghost particle with respect to a specific side used for reflective boundaries.
     *
     * @param p Particle to calculate the corresponding ghost particle of.
     * @param side Side of the domain at which the particle is mirrored.
     *
     * @return Position of the ghost particle.
     */
    std::array<double, 3> calcGhostParticle(Particle& p, Side side);

    /**
     * @brief Moves particles along a certain dimension from the halo cells back into the simulation domain.
     *
     * @param sideStart Side from which the particles will be teleported to the opposite side.
     * @param dimension Dimension along which the particles are moved (x := 0, y := 1, z := 2)
     * @param modus Specify if the particles are moved up or down along the dimension (down := 0, up := 1)
     *
     * Only updates the cell of the particle, if the particle is back into the domain in all three dimensions.
     */
    void teleportParticlesToOppositeSideHelper(Side sideStart, int dimension, int modus);

    /**
     * @brief Used for periodic boundaries to calculate the force that all particles from
     *        the opposite edge exert on the particles on the specified cell on the specified side.
     *
     * @param side Side on which the force is exerted.
     * @param cellToProcess Specific cell of that side.
     */
    void applyForceToOppositeCellsHelper(Side side, std::array<int,3> cellToProcess);

    /**
     * @brief Used for periodic boundaries to calculate the force that all particles
     *        from the opposite edge exert on the particles of the specified cell
     *
     * @param cellToProcess Cell on which particles the force is exerted.
     * @param offsetCell Offset to reach the cell of the opposite edge.
     * @param offsetPosition Offset to move the particles of the opposite edge to the current edge for proper force calculation.
     * @param dim Dimension in which the edge is living.
     */
    void applyForceToOppositeEdgeHelper(std::array<int, 3> cellToProcess, std::array<int, 3> offsetCell, std::array<double,3> offsetPosition , int dim);

    /**
     * @brief Checks, if the specified cell is part of the domain.
     *
     * @param cell Cell to check.
     * @return True if cell is part of the domain, false otherwise.
     */
    [[nodiscard]] bool isCellInDomain(std::array<int,3> cell) const;

    /**
     * @brief Checks, if the specified particle is inside the domain.
     *
     * @param position Position of the particle.
     * @return True if the particle is inside the domain, false otherwise.
     */
    [[nodiscard]] bool isParticleInDomain(const std::array<double, 3>& position) const;

    /**
     * @brief Calculates the forces that particles in the cell source apply to the particles in cell target.
     *
     * @param cellTarget Index of the cell containing the particles to which the force is applied.
     * @param cellSource Index of the cell containing the particles which exert the force.
     * @param offsetSource Offset to move the particles of the cell source in the right position for force calculations.
     */
    void applyForcesBetweenTwoCells(int cellTarget, int cellSource, std::array<double, 3> offsetSource);


public:
    /**
     * Constructor and methods
     */

    /**
     * @brief Constructor of the LinkedCellsContainer class
     *
     * @param domainSize Size of the simulation domain. Syntax {x, y, z}. Front lower left corner is by definition (0,0,0).
     * @param rCutOff The cut-off radius
     */

    LinkedCellsContainer(std::array<double, 3> domainSize, double rCutOff, BoundarySet boundarySet);

    /**
     * @brief Calculate the index of the cell to which a particle decided by its position belongs.
     *
     * @param position Position of the particle to calculate the cell index of.
     *
     * @return Corresponding cell index.
     */
    int calcCellIndex(const std::array<double, 3>& position);

    /**
     * @brief Add a particle to this container.
     *
     * @param p Particle to be added.
     *
     * If the simulation environment is 2D, only particles living in 2D space are accepted. Try adding particles living in 3D space
     * to 2D simulation environment will lead to the termination of the program.
     */
    void add(Particle& p) override;

    /**
     * @brief Convert 3 dimensional coordinates to one dimensional coordinates.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param z z coordinate
     *
     * @return Corresponding coordinate in one dimensional space.
     */
    [[nodiscard]] int threeDToOneD(int x, int y, int z) const;

    /**
     * @brief Convert a one dimensional coordinate to three dimensional coordinates.
     *
     * @param index One dimensional coordinate
     *
     * @return Corresponding coordinates in three dimensional space.
     */
    [[nodiscard]] std::array<int, 3> oneDToThreeD(int index) const;

    /**
     * @brief Assign each particle to its correct cell after there positions have been changed.
     */
    void updateCells();

    /**
     * @brief Delete all particles in all halo cells being part of a specific size.
     *
     * @param side Side at which all halo cells are cleared.
     */
    void clearHaloCells(Side side);

    /**
     * @brief Iterate over all particles in this container and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each particle.
     */
    void applyToEachParticle(const std::function<void(Particle &)> &function) override;

    /**
     * @brief Iterate over all particles that are part of the domain and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each particle.
     */
    void applyToEachParticleInDomain(const std::function<void(Particle &)> &function) override;

    /**
     * @brief Iterate over all unique pairs of particles being part of the simulation domain
     *        and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each unique pair of particles.
     *
     * The purpose of this function is provide an easy way of calculating the force between all particles by using
     * Newton's third law of motion.
     */

    void applyToAllUniquePairsInDomain(const std::function<void(Particle &, Particle &)> &function) override;

    /**
     * @brief Implements the optimization we presented as our second idea.
     *        At the moment this is dead code, because we did not have time yet to make it compatible
     *        with our current program structure and only integrated it once when doing the time measurements.
     */

    void applyToAllUniquePairsInDomainParallelOne(const std::function<void(Particle &, Particle &)> &function);

    void applyToAllUniquePairsInDomainOptimized(const std::function<void(Particle &, Particle &, std::array<double, 3>, double)> &function);

    /**
     * @brief Iterate over all particles in the boundary cells of a specific side
     *        which have a distance to that side that is smaller or equal than the threshold.
     *
     * @param function Lambda function that is applied to each particle fulfilling the requirements.
     * @param boundary Side to which the boundary cells belong.
     *
     * Additionally, the position of the corresponding ghost particle of each particle is calculated
     *        and passed in the lambda function as second input. This information makes the implementation
     *        of reflective boundaries a lot easier.
     */
    void applyToAllBoundaryParticles(const std::function<void(Particle &, std::array<double, 3>&)> &function, Side boundary);

    /**
     * @brief Get the number of particles stored in this container.
     *
     * @return Number of particles stored in this container.
     */
    [[nodiscard]] size_t size() const override;

    /**
     * @brief Move a particle along a certain dimension that underflows the domain int that dimension
     *        to the opposite side of the domain in that dimension.
     *
     * @param position Current position of the particle.
     * @param dimension Dimension along the particle is moved.
     *
     * @return New position.
     */
    std::array<double,3> fromLowToHigh(const std::array<double,3>& position, int dimension);

    /**
     * @brief Move a particle along a certain dimension that overflows the domain int that dimension
     *        to the opposite side of the domain in that dimension.
     *
     * @param position Current position of the particle.
     * @param dimension Dimension along the particle is moved.
     *
     * @return New position.
     */
    std::array<double,3> fromHighToLow(const std::array<double,3>& position, int dimension);

    /**
     * @brief Moves particles along a certain dimension from the halo cells back into the simulation domain.
     *
     * @param sideStart Side from which the particles will be teleported to the opposite side.
     *
     * Only updates the cell of the particle, if the particle is back into the domain in all three dimensions.
     */

    void teleportParticlesToOppositeSide(Side sideStart);


    /**
     * @brief Used for periodic boundaries to calculate the force that all particles from
     *        the opposite edge exert on the particles on the specified cell on the specified side.
     *
     * @param side Side on which the force is exerted.
     */
    void applyForcesFromOppositeSide(Side side);


    //Getter and setters. Especially the setters should only by used for testing purposes.

    std::vector<std::vector<Particle>>& getCells(){
        return cells;
    }

    std::array<std::vector<int>,6>& getHaloCells(){
        return haloCells;
    }

   std::array<std::vector<int>,6>& getBoundaries(){
        return boundaries;
    }

    std::vector<std::vector<int>>& getDomainCellIterationScheme(){
        return domainCellIterationScheme;
    }

    [[nodiscard]] int getNX() const {
        return nX;
    }

    [[nodiscard]] int getNY() const {
        return nY;
    }

    [[nodiscard]] int getNZ() const {
        return nZ;
    }

    [[nodiscard]] double getCellSizeX() const {
        return cellSizeX;
    }

    [[nodiscard]] double getCellSizeY() const {
        return cellSizeY;
    }

    [[nodiscard]] double getCellSizeZ() const {
        return cellSizeZ;
    }

    [[nodiscard]] double getRCutOff() const {
        return rCutOff;
    }

    [[nodiscard]] bool isTwoD() const {
        return twoD;
    }

    [[nodiscard]] std::array<double, 3> getDomainSize() const {
        return domainSize;
    }
};

