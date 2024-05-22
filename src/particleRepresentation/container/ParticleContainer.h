//
// Created by daniel on 22.05.24.
//

#pragma once
#include "particleRepresentation/particle/Particle.h"

class ParticleContainer {
public:

    virtual void add(Particle& p) = 0;

    virtual size_t size() = 0;

    virtual void applyToEachParticle(const std::function<void(Particle&)> &function) = 0;

    virtual void applyToEachParticleInDomain(const std::function<void(Particle&)> &function) = 0;

    virtual void applyToAllUniquePairsInDomain(const std::function<void(Particle&, Particle&)> &function) = 0;

    virtual ~ParticleContainer() = default;
};
