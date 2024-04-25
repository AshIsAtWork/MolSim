//
// Created by daniel on 24.04.24.
//

#pragma once

#include "Particle.h"

#include <vector>
#include <iterator>

/**
 * @brief Class to store the particles for simulation.
 *
 * The storation of the particles is based on std::vector. This guarantees fast iteration over the particles, because
 * they are stored consecutive in memory.
 */

class ParticleContainer {
private:
    std::vector<Particle> particles;

public:
    /**
     * @brief Iterator for ParticleContainer
     *
     * This class provides an iterator implementation for the class Particle Container.
     */
    struct Iterator {
        //Iterator tags

        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Particle;
        using pointer = Particle*;
        using reference = Particle&;

        //Iterator constructors
        Iterator(pointer ptr) : m_ptr(ptr){}

        //Pointer and Reference

        reference operator*() const {return *m_ptr;}
        pointer operator->() {return m_ptr;}

        //Prefix increment
        Iterator& operator++() {m_ptr++; return *this;}

        //Postfix increment
        Iterator operator++(int){ Iterator tmp = *this; ++(*this); return tmp;}

        //Comparison
        friend bool operator==(const Iterator& a, const Iterator& b){ return a.m_ptr == b.m_ptr;}
        friend bool operator!=(const Iterator& a, const Iterator& b){ return a.m_ptr != b.m_ptr;}

    private:

        pointer m_ptr;
    };

    ParticleContainer() = default;

    /**
     * @brief Create a ParticleContainer which can store up to capacity particles.
     *
     * @param capacity
     *
     * If the capacity is exceeded the whole data structure will be extended leading to reallocation.
     */
    explicit ParticleContainer(size_t capacity);

    /**
     * @brief Add one particle to this container.
     *
     * @param p Particle to be added to this container.
     *
     * Add one particle to this container. It will be appended.
     */
    void add(Particle &p);

    /**
     * @brief Obtain particle at position i.
     *
     * @param i Position of the particle of concern.
     * @return Particle at position i.
     *
     * Before access, it will be checked, if the position is valid. Otherwise an exeption will be thrown.
     */
    Particle& at(size_t i);

    /**
     * @brief Retrieve the size of the data structure.
     *
     * @return Number of particles in this container.
     *
     * Provides the number of particles currently stored in the data structure.
     */
    size_t size();

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
    Iterator begin();

    /**
     * @brief Obtain an iterator to the last element of this container.
     *
     * @return Iterator to last element of this container.
     *
     * Obtain an iterator to the last element of this container.
     */
    Iterator end();
};

