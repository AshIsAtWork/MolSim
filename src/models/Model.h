//
// Created by daniel on 22.05.24.
//

#pragma once
#include "moleculeSimulator/forceCalculation/Force.h"
#include "particleRepresentation/container/ParticleContainer.h"

class Model {
protected:
    ParticleContainer &particles;
    Force &force;
    double deltaT;

    Model(ParticleContainer &particles, Force &force, double deltaT);

    void updatePositions() const;

    void updateVelocities() const;

public:
    ParticleContainer & getParticles() {
        return particles;
    }

    void addCuboid(const std::array<double, 3> &position, unsigned N1, unsigned N2, unsigned N3, double h, double mass,
                   const std::array<double, 3> &initVelocity);

    void addParticle(Particle& p);

    void updateForces() const;

    virtual void step() = 0;

    virtual ~Model() = default;
};
