//
// Created by daniel on 24.04.24.
//

#include "ParticleContainer.h"


ParticleContainer::ParticleContainer(size_t capacity) {
    particles.reserve(capacity);
}

void ParticleContainer::add(Particle p) {
    particles.push_back(std::move(p));
}

Particle &ParticleContainer::at(size_t i) {
    return particles.at(i);
}

size_t ParticleContainer::size() {
    return particles.size();
}

size_t ParticleContainer::capacity() {
    return particles.capacity();
}

std::vector<Particle>::iterator ParticleContainer::begin() {
    return particles.begin();
}

std::vector<Particle>::iterator ParticleContainer::end() {
    return particles.end();
}

void ParticleContainer::reserve(size_t n) {
    particles.reserve(n);
}
