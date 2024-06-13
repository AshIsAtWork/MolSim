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
    /**
     *  Data structure:
     *
     *  -We use an 1D vector to store the flattened 3D cell structure
     *   being an essential property of the linked cells algorithm. Each cell is represented itself by an 1D vector of particles.
     *
     *  -The current number of particles that is contained in this container is tracked by the attribute currentSize and kept up-to-date
     *   through every operation.
     */
    std::vector<std::vector<Particle>> cells;
    size_t currentSize;

    /**
     *  Precomputed indizes:
     *
     *  The indizes of
     *                 -all halo cells belonging to one or more of the 6 sides,
     *                 -all boundary cells belonging to one or more of the 6 sides,
     *                 -the order in which all cells within the domain are processed to process each pair of particles only ones
     *
     *  are precomputed in the constructor and stored in memory to provide fast access in future iterations. We invest here additional
     *  memory space to avoid recomputing all indizes in each iteration again.
     */
    std::array<std::vector<int>, 6> haloCells;
    std::array<std::vector<int>, 6> boundaries;
    std::vector<std::vector<int>> domainCellIterationScheme;

    /**
     * All parameters used in this model:
     *                                      -nX : number of cells in dimension x
     *                                      -nY : number of cells in dimension y
     *                                      -nZ : number of cells in dimension z
     *                                      -baseY : number of cells in one row (nX many)
     *                                      -baseZ : number of cells in one layer (nX * nY many)
     *                                      -cellSizeX : size of each cell in dimension x
     *                                      -cellSizeY : size of each cell in dimension y
     *                                      -cellSizeZ : size of each cell in dimension z
     *                                      -rCutOff : cut-off radius
     *                                      -twoD : specifies, if the simulation only uses 2 of 3 dimensions.
     *                                              If this is the case, resources can be saved.
     *                                      -domainSize : Size of the domain {x, y , z}. The front lower left corner
     *                                                    of the domain is set to (0,0,0) by definition.
     */
    int nX;
    int nY;
    int nZ;
    int baseY;
    int baseZ;
    double cellSizeX;
    double cellSizeY;
    double cellSizeZ;
    double rCutOff;
    bool twoD;
    std::array<double, 3> domainSize;

    outputWriter::VTKWriter vtk_writer;

    LeonardJonesForce lJF;

    BoundarySet boundarieSet;



    /**
     * Helper methods for index calculation and boundary conditions.
     */

    /**
     * @brief Precalculation of all halo cell indizes.
     */
    void calculateHaloCellIndizes();

    /**
     * @brief Precalculation of all boundary cell indizes.
     */
    void calculateBoundaryCellIndizes();

    /**
     * @brief Precalculation of the indizes defining the processing order of cells.
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

    void teleportParticlesToOppositeSideHelper(Side sideStart, int dimension, int modus);

    void applyForceToOppositeCellsHelper(Side side, std::array<int,3> cellToProcess);

    void applyForceToOppositeEgdeHelper(std::array<int, 3> cellToProcess, std::array<int, 3> offsetCell,std::array<double,3> offsetPosition ,int dim);

    bool isCellInDomain(std::array<int,3> cell) const;

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

    LinkedCellsContainer(std::array<double, 3> domainSize, double rCutOff);

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
    int threeDToOneD(int x, int y, int z) const;

    /**
     * @brief Convert a one dimensional coordinate to three dimensional coordinates.
     *
     * @param index One dimensional coordinate
     *
     * @return Corresponding coordinates in three dimensional space.
     */
    std::array<int, 3> oneDToThreeD(int index) const;

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
     * @brief Iterate over all particles in the boundry cells of a specific side
     *        which have a distance to that side that is smaller or equal than the threshold.
     *
     * @param function Lambda function that is applied to each particle fulfilling the requirements.
     * @param boundary Side to which the boundary cells belong.
     * @param threshold Maximal distance to the side.
     *
     * Additionally, the position of the corresponding ghost particle of each particle is calculated
     *        and passed in the lambda function as second input. This information makes the implementation
     *        of reflective boundaries a lot easier.
     */
    void applyToAllBoundaryParticles(const std::function<void(Particle &, std::array<double, 3>&)> &function, Side boundary, double threshold);

    /**
     * @brief Get the number of particles stored in this container.
     *
     * @return Number of particles stored in this container.
     */
    size_t size() override;

    /**
     * Getter and setters. Especially the setters should only by used for testing purposes.
     */

    std::array<double,3> fromLowToHigh(const std::array<double,3>& position, int dimension);

    std::array<double,3> fromHighToLow(const std::array<double,3>& position, int dimension);

    void teleportParticlesToOppositeSide(Side sideStart);

    void applyForcesFromOppositeSide(Side side);

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

