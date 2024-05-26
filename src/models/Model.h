//
// Created by daniel on 22.05.24.
//

#pragma once
#include "fileHandling/FileHandler.h"
#include "moleculeSimulator/forceCalculation/Force.h"
#include "particleRepresentation/container/ParticleContainer.h"

class Model {
private:
    FileHandler fileHandler;
    FileHandler::inputFormat inputFormat;
    FileHandler::outputFormat outputFormat;

protected:
    ParticleContainer &particles;
    Force &force;
    double deltaT;

    Model(ParticleContainer &particles, Force &force, double deltaT,  FileHandler::inputFormat inputFormat,
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
    void plot(int iteration);

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
     */
    void addCuboid(const std::array<double, 3> &position, unsigned N1, unsigned N2, unsigned N3, double h, double mass,
                   const std::array<double, 3> &initVelocity, int dimensions);

     /**
     * @brief Add a 2D disc structure to this model.
     *
     * @param center The coordinates of the center of the disc.
     * @param initVelocity Initial velocity of the of the particles.
     * @param N Number of particles along the radius, including the particle in the center.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     * * @param dimensions Number of dimensions to which the Brownian Motion will be added. Valid values are 0, 1, 2 and 3.
     */
    void addDisc(const std::array<double, 3> &center,
                             const std::array<double, 3> &initVelocity, int N, double h, double mass, int dimensions);

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

    virtual ~Model() = default;

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
