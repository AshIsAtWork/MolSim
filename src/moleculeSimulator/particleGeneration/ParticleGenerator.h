//
// Created by daniel on 04.05.24.
//

#pragma once
#include "particleRepresentation/ParticleContainer.h"

class ParticleGenerator {
private:
    const static double brownianMotionAverageVelocity;
    static int id;

public:
    static void generateCuboid(ParticleContainer& particles,const std::array<double,3>& position, unsigned N1, unsigned N2, unsigned N3,
        double h, double mass, const std::array<double,3>& velocity);

};


