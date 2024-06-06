//
// Created by daniel on 24.04.24.
//

#include "DefaultParticleContainer.h"


DefaultParticleContainer::DefaultParticleContainer(size_t capacity) {
    particles.reserve(capacity);
}

void DefaultParticleContainer::add(Particle &p) {
    particles.push_back(std::move(p));
}

Particle &DefaultParticleContainer::at(size_t i) {
    return particles.at(i);
}

size_t DefaultParticleContainer::size() {
    return particles.size();
}

size_t DefaultParticleContainer::capacity() {
    return particles.capacity();
}

std::vector<Particle>::iterator DefaultParticleContainer::begin() {
    return particles.begin();
}

std::vector<Particle>::iterator DefaultParticleContainer::end() {
    return particles.end();
}

void DefaultParticleContainer::reserve(size_t n) {
    particles.reserve(n);
}

bool DefaultParticleContainer::contains(Particle &p) {
    for (Particle &toCheck: particles) {
        if (toCheck == p) {
            return true;
        }
    }
    return false;
}

void DefaultParticleContainer::applyToEachParticle(const std::function<void(Particle &)> &function) {
    for(Particle& p : particles) {
        function(p);
    }
}

void DefaultParticleContainer::applyToEachParticleInDomain(const std::function<void(Particle &)> &function) {
    applyToEachParticle(function);
}

void DefaultParticleContainer::applyToAllUniquePairsInDomain(
    const std::function<void(Particle &, Particle &)> &function) {
    for (auto p_i = particles.begin(); p_i != particles.end(); std::advance(p_i,1)) {
        for (auto p_j = std::next(p_i); p_j != particles.end(); std::advance(p_j,1)) {
           function(*p_i, *p_j);
        }
    }
}


