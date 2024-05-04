//
// Created by daniel on 04.05.24.
//

#include "ParticleGenerator.h"

#include "utils/MaxwellBoltzmannDistribution.h"

const double ParticleGenerator::brownianMotionAverageVelocity = 0.1;
int ParticleGenerator::id = 1;

void ParticleGenerator::generateCuboid(ParticleContainer &particles, const std::array<double, 3>& position, unsigned N1,
                                       unsigned N2, unsigned N3, double h, double mass,const std::array<double, 3>& velocity) {

    std::array<double, 3> currentPosition = position;

    for(unsigned n1 = 0; n1 < N1; n1++) {
        for(unsigned n2 = 0; n2 < N2; n2++) {
            for(unsigned n3 = 0; n3 < N3; n3++) {
                Particle newParticle {currentPosition,
                    maxwellBoltzmannDistributedVelocity(brownianMotionAverageVelocity,3) + velocity, mass, ParticleGenerator::id};
                particles.add(newParticle);
                currentPosition[2] += h;
            }
            currentPosition[1] += h;
            currentPosition[2] = position[2];
        }
        currentPosition[0] += h;
        currentPosition[1] = position[1];
        currentPosition[2] = position[2];
    }
}
