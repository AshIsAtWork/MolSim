//
// Created by daniel on 22.05.24.
//

#pragma once
#include "particleRepresentation/particle/Particle.h"

/**
 * Abstract base class for a particle container. Each type of a particle container should extend this class.
 */
class ParticleContainer {
public:

    /**
     * @brief Add one particle to this container.
     *
     * @param p Particle to be added to this container.
     *
     * Add one particle to this container. It will be appended.
     */

    virtual void add(Particle& p) = 0;

    /**
     * @brief Get the number of particles stored in this container.
     *
     * @return Number of particles stored in this container.
     */
    virtual size_t size() = 0;

    /**
     * @brief Iterate over all particles in this container and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each particle.
     */
    virtual void applyToEachParticle(const std::function<void(Particle&)> &function) = 0;

    /**
     * @brief Iterate over all particles that are part of the domain and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each particle.
     */
    virtual void applyToEachParticleInDomain(const std::function<void(Particle&)> &function) = 0;

    /**
    * @brief Iterate over all unique pairs of particles being part of the simulation domain
    *        and apply a lambda function to them.
    *
    * @param function Lambda function that is applied to each unique pair of particles.
    *
    * The purpose of this function is provide an easy way of calculating the force between all particles by using
    * Newton's third law of motion.
    */
    virtual void applyToAllUniquePairsInDomain(const std::function<void(Particle&, Particle&)> &function) = 0;

   /**
    *@brief Virtual default constructor to guarantee appropriate memory clean up
    */
    virtual ~ParticleContainer() = default;
};
