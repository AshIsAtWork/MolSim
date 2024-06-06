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
    constexpr  static double brownianMotionAverageVelocity = 0.1;

    /**
     * All particles being part of the same structure will be assigned the same id. This id is incremented by
     * one for each structure that is created.
     */
    static int id;

public:
    /**
     * @brief Generate a cuboid structure.
     *
     * @param particles Particle container to which the particle structure is added.
     * @param position The coordinate of the lower left front-side corner.
     * @param N1 Number of particles in dimension N1.
     * @param N2 Number of particles in dimension N2.
     * @param N3 Number of particles in dimension N3.
     * @param h Distance of the particles (mesh width of the grid).
     * @param mass Mass of one particle.
     * @param initVelocity Initial velocity of the of the particles.
     */
    static void generateCuboid(ParticleContainer& particles,const std::array<double,3>& position, unsigned N1, unsigned N2, unsigned N3,
                               double h, double mass, const std::array<double,3>& initVelocity);

};


