//
// Created by daniel on 22.05.24.
//

#pragma once
#include "fileHandling/FileHandler.h"
#include "moleculeSimulator/forceCalculation/Force.h"
#include "particleRepresentation/container/ParticleContainer.h"

/**
 * @brief Abstract base class for any model for molecule simulation.
 *
 * Each specific model should extend this class. At the moment there are two different models:
 * -Direct Sum
 * -Linked Cells
 */
class Model {
private:
    FileHandler fileHandler;
    FileHandler::inputFormat inputFormat;
    FileHandler::outputFormat outputFormat;

protected:
    ParticleContainer &particles;
    Force &force;
    double deltaT;

    /**
     * @brief Constructor for this model. Cannot be called from any other class but classes that extend this class,
     *        because this class is a base class and should not be instantiated
     *
     * @param particles Particle container of the model. This parameter is instantiated with the appropriated container by the derived models.
     * @param force Force to use in the simulation.
     * @param deltaT Duration of one time step. Small time step will result in a better simulation, but will demand more computational resources.
     * @param inputFormat Format of the input file. Supported formats are txt and xml.
     * @param outputFormat Format of the output file. Supported formats are vtk and xyz.
     */
    Model(ParticleContainer &particles, Force &force, double deltaT, FileHandler::inputFormat inputFormat,
          FileHandler::outputFormat outputFormat);

    /**
    * @brief Helper method to calculate the position of all particles.
    *
    * After each time step positions of all particles with velocities unequal to zero have to be updated.
    */

    void updatePositions() const;

    /**
    * @brief Helper method to calculate the velocity of all particles.
    *
    * After each time step the velocity of each particle may have changed due to forces that act between the particles.
    */

    void updateVelocities() const;

public:
    /**
     *@brief Virtual default constructor to guarantee appropriate memory clean up
     */
    virtual ~Model() = default;

    /**
     * @brief Write current state of the model to a file.
     *
     * @param iteration Current iteration.
     * @param baseName Base name of the output file.
     */
    void plot(int iteration, std::string& baseName);

    /**
     * @brief Add a cuboid structure to this model.
     *
     * @param position The coordinates of the lower left front-side corner.
     * @param N1 Number of particles in dimension N1.
     * @param N2 Number of particles in dimension N2.
     * @param N3 Number of particles in dimension N3.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     * @param initVelocity Initial velocity of the of the particles.
     * @param dimensions Number of dimensions to which the Brownian Motion will be added. Valid values are 0, 1, 2 and 3.
     * @param brownianMotionAverageVelocity
     */
    void addCuboid(const std::array<double, 3> &position, unsigned N1, unsigned N2, unsigned N3, double h, double mass,
                   const std::array<double, 3> &initVelocity, int dimensions, double brownianMotionAverageVelocity);

    /**
    * @brief Add a 2D disc structure to this model.
    *
    * @param center The coordinates of the center of the disc.
    * @param initVelocity Initial velocity of the of the particles.
    * @param N Number of particles along the radius, including the particle in the center.
    * @param h Distance of the particles (mesh width of the grid).
    * @param mass Mass of one particle.
    * @param dimensions Number of dimensions to which the Brownian Motion will be added. Valid values are 0, 1, 2 and 3.
    * @param brownianMotionAverageVelocity Constant, specifying the average velocity of the Brownian Motion.
    */
    void addDisc(const std::array<double, 3> &center,
                 const std::array<double, 3> &initVelocity, int N, double h, double mass, int dimensions,
                 double brownianMotionAverageVelocity);

    /**
     * @brief Add a single particle to this model.
     *
     * @param p Particle to add to this model.
     */

    void addParticle(Particle &p);

    /**
     * @brief Add new particles / particle structures to the model via a file
     *
     * @param filepath Path to the file. Example: ../input/eingabe-sonne.txt
     */
    void addViaFile(std::string &filepath);

    /**
    * @brief Helper method to calculate the force between all particles.
    *
    * After each time step the forces acting between the particles have changed due to their new positions, so
    * they have to be recalculated. This method uses Newtons third law of motion to simplify calculations and make
    * them more efficient.
    */

    void updateForces() const;

    /**
     * @brief Perform one single step in the simulation.
     *
     * Each step consists of force, velocity and position updates. For each concrete model the exact
     * implementation might differ.
     */

    virtual void step() = 0;

    /**
     * @brief Get the Particles of this model.
     *
     * @return Particles stored in this model.
     *
     * Should be only used for testing purposes.
     */
    [[nodiscard]] ParticleContainer &getParticles() const {
        return particles;
    };
};
