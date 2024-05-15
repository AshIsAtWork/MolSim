//
// Created by daniel on 04.05.24.
//

#include "ParticleGenerator.h"

//The particles of the first cuboid will get all get id 1, the particles of the next cuboid id 2 and so on.

int ParticleGenerator::id = 1;

void ParticleGenerator::generateCuboid(ParticleContainer &particles, const std::array<double, 3> &position, unsigned N1,
                                       unsigned N2, unsigned N3, double h, double mass,
                                       const std::array<double, 3> &initVelocity) {
    spdlog::info("Generate cuboid with the following parameters:\n"
                 "Position: {}\n"
                 "N1: {}\n"
                 "N2: {}\n"
                 "N3: {}\n"
                 "h: {}\n"
                 "m: {}\n"
                 "Velocity: {}\n",
                 ArrayUtils::to_string(position), N1, N2, N3, h, mass, ArrayUtils::to_string(initVelocity)
    );

    std::array<double, 3> currentPosition = position;

    for (unsigned n1 = 0; n1 < N1; n1++) {
        for (unsigned n2 = 0; n2 < N2; n2++) {
            for (unsigned n3 = 0; n3 < N3; n3++) {
                Particle pToAdd = {
                    currentPosition,
                    maxwellBoltzmannDistributedVelocity(brownianMotionAverageVelocity, 3) + initVelocity,
                    mass,
                    id
                };
                particles.add(pToAdd);
                currentPosition[2] += h;
            }
            currentPosition[1] += h;
            currentPosition[2] = position[2];
        }
        currentPosition = {currentPosition[0] + h, position[1], position[2]};
    }
    //Increment id, so that all particles of the next cuboid being generated will receive another id.
    ++id;
}
