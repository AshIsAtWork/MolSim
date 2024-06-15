//
// Created by daniel on 04.05.24.
//

#include "ParticleGenerator.h"

//The particles of the first cuboid will all get id 1, the particles of the next cuboid id 2 and so on.

int ParticleGenerator::id = 1;

void ParticleGenerator::generateDiscQuadrant(ParticleContainer &particles, const std::array<double, 3> &corner,
                                             const std::array<double, 3> &initVelocity, double h, double mass,
                                             const int N, double r, int dimensions,
                                             double brownianMotionAverageVelocity, std::array<int, 4> transformMatrix,
                                             double epsilon, double sigma) {
    std::array<double, 3> currentPosition = corner;
    for (int d1 = 0; d1 < N - 1; d1++) {
        int threshold2D = static_cast<int>(std::floor(std::sqrt(std::abs(std::pow((d1 + 1) * h, 2) - std::pow(r, 2))))
                                           / h) + 1;
        for (int d2 = 0; d2 < threshold2D; d2++) {
            Particle pToAdd = {
                currentPosition,
                initVelocity + maxwellBoltzmannDistributedVelocity(brownianMotionAverageVelocity, dimensions),
                mass,
                id,
                epsilon,
                sigma
            };
            particles.add(pToAdd);
            currentPosition[transformMatrix[1]/*d2*/] += h * transformMatrix[3]/*sign d2*/;
        }
        currentPosition[transformMatrix[0]/*d1*/] += h * transformMatrix[2]/*sign d1*/;
        currentPosition[transformMatrix[1]/*d2*/] = corner[transformMatrix[1]/*d2*/];
    }
}

void ParticleGenerator::generateCuboid(ParticleContainer &particles, const std::array<double, 3> &position, unsigned N1,
                                       unsigned N2, unsigned N3, double h, double mass,
                                       const std::array<double, 3> &initVelocity, int dimensions,
                                       double brownianMotionAverageVelocity, double epsilon, double sigma) {
    spdlog::info("Generate cuboid with the following parameters:\n"
                 "Position: {}\n"
                 "N1: {}\n"
                 "N2: {}\n"
                 "N3: {}\n"
                 "h: {}\n"
                 "m: {}\n"
                 "Velocity: {}\n"
                 "Brownian Motion: {}D\n",
                 "Epsilon: {}D\n",
                 "Sigma {}D\n",
                 ArrayUtils::to_string(position), N1, N2, N3, h, mass, ArrayUtils::to_string(initVelocity), dimensions,
                 epsilon, sigma
    );

    std::array<double, 3> currentPosition = position;

    for (unsigned n1 = 0; n1 < N1; n1++) {
        for (unsigned n2 = 0; n2 < N2; n2++) {
            for (unsigned n3 = 0; n3 < N3; n3++) {
                Particle pToAdd = {
                    currentPosition,
                    maxwellBoltzmannDistributedVelocity(brownianMotionAverageVelocity, dimensions) + initVelocity,
                    mass,
                    id,
                    epsilon,
                    sigma
                };
                particles.add(pToAdd);
                currentPosition[2] += h;
            }
            currentPosition[1] += h;
            currentPosition[2] = position[2];
        }
        currentPosition = {currentPosition[0] + h, position[1], position[2]};
    }
    //Increment id, so that all particles of the next body being generated will receive another id.
    ++id;
}

void ParticleGenerator::generateDisc(ParticleContainer &particles, const std::array<double, 3> &center,
                                     const std::array<double, 3> &initVelocity, int N, double h, double mass,
                                     int dimensions, double brownianMotionAverageVelocity, double epsilon,
                                     double sigma) {
    if (N == 0) {
        return;
    }

    spdlog::info("Generate disc with the following parameters:\n"
                 "Center: {}\n"
                 "N: {}\n"
                 "h: {}\n"
                 "m: {}\n"
                 "Velocity: {}\n"
                 "Brownian Motion: {}D\n",
                 "Epsilon: {}D\n",
                 "Sigma {}D\n",
                 ArrayUtils::to_string(center), N, h, mass, ArrayUtils::to_string(initVelocity), dimensions, epsilon,
                 sigma
    );

    //First add the particle in the center
    Particle centerParticle = {center, initVelocity, mass, id};
    particles.add(centerParticle);

    std::array<double, 3> currentPosition = center;

    //Calculate radius
    double r = h * (N - 1);
    //Generate upper right quadrant
    currentPosition[1] += h;
    generateDiscQuadrant(particles, currentPosition, initVelocity, h, mass, N, r, dimensions,
                         brownianMotionAverageVelocity, {1, 0, 1, 1}, epsilon, sigma);

    //Generate lower left quadrant
    currentPosition[1] -= 2 * h;
    generateDiscQuadrant(particles, currentPosition, initVelocity, h, mass, N, r, dimensions,
                         brownianMotionAverageVelocity, {1, 0, -1, -1}, epsilon, sigma);

    //Generate lower right quadrant
    currentPosition[1] += h;
    currentPosition[0] += h;
    generateDiscQuadrant(particles, currentPosition, initVelocity, h, mass, N, r, dimensions,
                         brownianMotionAverageVelocity, {0, 1, 1, -1}, epsilon, sigma);

    //Generate upper left quadrant
    currentPosition[0] -= 2 * h;
    generateDiscQuadrant(particles, currentPosition, initVelocity, h, mass, N, r, dimensions,
                         brownianMotionAverageVelocity, {0, 1, -1, 1}, epsilon, sigma);
    //Increment id, so that all particles of the next body being generated will receive another id.
    id++;
}
