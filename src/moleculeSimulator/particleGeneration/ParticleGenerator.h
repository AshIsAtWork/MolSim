//
// Created by daniel on 04.05.24.
//

#pragma once
#include "particleRepresentation/ParticleContainer.h"
#include "utils/MaxwellBoltzmannDistribution.h"

/**
 * @brief Class for generating various particle structures. Currently only cuboids are supported.
 */
class ParticleGenerator {
private:
    /**
     * At the moment this parameter is fixed. In future versions this may change.
     */
    constexpr static double brownianMotionAverageVelocity = 0.1;

    /**
     * All particles being part of the same structure will be assigned the same id. This id is incremented by
     * one for each structure that is created.
     */
    static int id;

    /**
     * @brief Helper method to generate one of four quadrants of an 2-dimensional disc structure.
     *
     * @param particles Particle container to which the particle structure is added.
     * @param corner The coordinates of the corner of the quadrant.
     * @param initVelocity Initial velocity of the of the particles.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     * @param N Number of particles along the radius, including the particle in the center.
     * @param r Length of the radius of the circle
     * @param transformMatrix Consists of four parameters: [0]: main axis d1 (0 for x-axis, 1 for y-axis)
     *                                                     [1]: support axis d2 (0 for x-axis, 1 for y-axis)
     *                                                     [2]: direction along main axis d1 (0 for positive, 1 for negative)
     *                                                     [3]: direction along support axis d2 (0 for positive, 1 for negative)
     */
    static void generateDiscQuadrant(ParticleContainer &particles, const std::array<double, 3> &corner,
                                     const std::array<double, 3> &initVelocity, double h, double mass, int N, double r,
                                     std::array<int, 4> transformMatrix);

public:
    /**
     * @brief Generate a cuboid structure.
     *
     * @param particles Particle container to which the particle structure is added.
     * @param position The coordinates of the lower left front-side corner.
     * @param N1 Number of particles in dimension N1.
     * @param N2 Number of particles in dimension N2.
     * @param N3 Number of particles in dimension N3.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     * @param initVelocity Initial velocity of the of the particles.
     */
    static void generateCuboid(ParticleContainer &particles, const std::array<double, 3> &position, unsigned N1,
                               unsigned N2, unsigned N3,
                               double h, double mass, const std::array<double, 3> &initVelocity);

    /**
     * @brief Generate a 2D disc structure.
     *
     * @param particles Particle container to which the particle structure is added.
     * @param center The coordinates of the center of the disc.
     * @param initVelocity Initial velocity of the of the particles.
     * @param N Number of particles along the radius, including the particle in the center.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     */
    static void generateDisc(ParticleContainer &particles, const std::array<double, 3> &center,
                             const std::array<double, 3> &initVelocity, int N, double h, double mass);
};
