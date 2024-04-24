//
// Created by daniel on 24.04.24.
//

#pragma once

#include "Particle.h"

#include <vector>
#include <iterator>


class ParticleContainer {
private:
    std::vector<Particle> particles;

public:

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
        Iterator& operator++(int){ Iterator tmp = *this; ++(*this); return tmp;}

        //Comparison
        friend bool operator==(const Iterator& a, const Iterator& b){ return a.m_ptr == b.m_ptr;}
        friend bool operator!=(const Iterator& a, const Iterator& b){ return a.m_ptr != b.m_ptr;}

    private:

        pointer m_ptr;
    };

    ParticleContainer() = default;

    explicit ParticleContainer(size_t capacity);

    void add(Particle &p);

    Particle& at(size_t i);

    size_t size();

    size_t capacity();

    Iterator begin();

    Iterator end();
};

