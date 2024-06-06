//
// Created by daniel on 24.04.24.
//

#pragma once

#include "particleRepresentation/particle/Particle.h"
#include <vector>

#include "../ParticleContainer.h"

/**
 * @brief Container to store the particles for simulation using the direct sum algorithm.
 *
 * The storage of the particles is based on std::vector. This guarantees fast iteration over the particles, because
 * they are stored consecutively in memory.
 */

class DefaultParticleContainer : public ParticleContainer {
private:
    std::vector<Particle> particles;

public:
    DefaultParticleContainer() = default;

    /**
     * @brief Create a ParticleContainer which can store up to capacity particles.
     *
     * @param capacity
     *
     * If the capacity is exceeded the whole data structure will be extended leading to reallocation.
     */
    explicit DefaultParticleContainer(size_t capacity);

    /**
     * @brief Add one particle to this container.
     *
     * @param p Particle to be added to this container.
     *
     * Add one particle to this container. It will be appended.
     */
    void add(Particle &p) override;

    /**
     * @brief Obtain particle at position i.
     *
     * @param i Position of the particle of concern.
     * @return Particle at position i.
     *
     * Before access, it will be checked, if the position is valid. Otherwise an exception will be thrown.
     */
    Particle& at(size_t i);

    /**
     * @brief Retrieve the size of the data structure.
     *
     * @return Number of particles in this container.
     *
     * Provides the number of particles currently stored in the data structure.
     */
    size_t size() override;

    /**
     * @brief Retrieve the capacity of this container.
     *
     * @return Capacity of this container.
     *
     * The capacity of this container is the number of particles which can be stored, before the data structure has to
     * be extended.
     */
    size_t capacity();

    /**
     * @brief Obtain an iterator to the first particle in this container.
     *
     * @return Iterator to first particle in this container.
     *
     * Obtain an iterator to the first particle in this container.
     */
    std::vector<Particle>::iterator begin();

    /**
     * @brief Obtain an iterator to the last element of this container.
     *
     * @return Iterator to last element of this container.
     *
     * Obtain an iterator to the last element of this container.
     */
    std::vector<Particle>::iterator end();

    /**
     * @brief Used to reserve the capacity of the vector used to storing particles
     *
     * @param n desired size
     */
    void reserve(size_t n);

    /**
     * @brief Check, if this particle container contains an particle p' that equals p.
     *
     * @param p Particle to look for
     * @return true, if this particle container contains some particle p' that equals p, false otherwise.
     *
     * This method should only be used for testing purposes in small instances, because the implementation
     * is not that efficient.
     */
    bool contains(Particle &p);

    /**
     * @brief Iterate over all particles in this container and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each particle.
     */
    void applyToEachParticle(const std::function<void(Particle &)> &function) override;

    /**
     * @brief Iterate over all particles that are part of the domain and apply a lambda function to them.
     *        For this container this method does the same as applyToEachParticle(),
     *        because every particle is in the simulation domain.
     *
     * @param function Lambda function that is applied to each particle.
     */
    void applyToEachParticleInDomain(const std::function<void(Particle &)> &function) override;

    /**
     * @brief Iterate over all unique pairs of particles being part of the simulation domain
     *        (in this container all particles) and apply a lambda function to them.
     *
     * @param function Lambda function that is applied to each unique pair of particles.
     *
     * The purpose of this function is provide an easy way of calculating the force between all particles by using
     * Newton's third law of motion.
     */
    void applyToAllUniquePairsInDomain(const std::function<void(Particle &, Particle &)> &function) override;
};
