//
// Created by daniel on 24.04.24.
//

#include "ParticleContainer.h"


ParticleContainer::ParticleContainer(size_t capacity) {
    particles.reserve(capacity);
}

void ParticleContainer::add(Particle& p) {
    particles.push_back(p);
}

Particle& ParticleContainer::at(size_t i) {
    return particles.at(i);
}

size_t ParticleContainer::size() {
    return particles.size();
}

size_t ParticleContainer::capacity() {
    return particles.capacity();
}

ParticleContainer::Iterator ParticleContainer::begin() {
    return Iterator{&particles[0]};
}

ParticleContainer::Iterator ParticleContainer::end() {
    return Iterator{&particles[particles.size()]};
}



