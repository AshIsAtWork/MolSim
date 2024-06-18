//
// Created by daniel on 22.05.24.
//
#include "Model.h"

#include "moleculeSimulator/particleGeneration/ParticleGenerator.h"

Model::Model(ParticleContainer &particles, Force &force, double deltaT,
             FileHandler::outputFormat outputFormat, bool gravityOn, double g) : outputFormat{outputFormat}, particles{particles},
             force{force}, deltaT{deltaT}, gravityOn{gravityOn}, g{g} {
}

void Model::updateForces() const {
    //Before calculating the new forces, the current forces have to be reset.
    particles.applyToEachParticleInDomain([](Particle &p) {
        p.resetForce();
    });
    //Calculate new forces using Newtons third law of motion
    particles.applyToAllUniquePairsInDomain([this](Particle &p_i, Particle &p_j) {
        auto f_ij{force.compute(p_i, p_j)};
        p_i.f = p_i.f + f_ij;
        p_j.f = p_j.f - f_ij;
    });
}

void Model::updatePositions() const {
    particles.applyToEachParticleInDomain([this](Particle &p) {
        p.x = p.x + deltaT * p.v + ((deltaT * deltaT) / (2.0 * p.m)) * p.old_f;
    });
}

void Model::updateVelocities() const {
    particles.applyToEachParticleInDomain([this](Particle &p) {
        p.v = p.v + (deltaT / (2 * p.m)) * (p.old_f + p.f);
    });
}

void Model::applyGravity() {
    particles.applyToEachParticleInDomain([this](Particle &p) {
        auto force = p.f;
        force[1] += p.m * g;
        p.f = force;

    });
}

void Model::plot(int iteration, std::string &baseName) {
    fileHandler.writeToFile(particles, iteration, outputFormat, baseName);
}

void Model::addCuboid(const std::array<double, 3> &position, unsigned N1, unsigned N2,
                      unsigned N3, double h, double mass, const std::array<double, 3> &initVelocity, int dimensions,
                      double brownianMotionAverageVelocity, double epsilon, double sigma) {
    ParticleGenerator::generateCuboid(particles, position, N1, N2, N3, h, mass, initVelocity, dimensions,
                                      brownianMotionAverageVelocity, epsilon, sigma);
}

void Model::addDisc(const std::array<double, 3> &center,
                    const std::array<double, 3> &initVelocity, int N, double h, double mass, int dimensions,
                    double brownianMotionAverageVelocity, double epsilon, double sigma) {
    ParticleGenerator::generateDisc(particles, center, initVelocity, N, h, mass, dimensions,
                                    brownianMotionAverageVelocity, epsilon, sigma);
}

void Model::addParticle(Particle &p) {
    particles.add(p);
}

void Model::addViaFile(std::string &filepath, FileHandler::inputFormat inputFormat) {
    FileHandler::readFile(particles, filepath, inputFormat);
}

void Model::saveState() {
    //Before writing the molecules to a file, we offset their types by 100 to avoid type clashes when they are loaded into a new simulation
    //Be doing it this way you always know which particles have been loaded from another simulation and can easily distinguish them.
    particles.applyToEachParticle([](Particle& p){p.type += 100;});
    std::string name = "Checkpoint.txt";
    fileHandler.writeToFile(particles,0,FileHandler::outputFormat::txt,name);
    particles.applyToEachParticle([](Particle& p){p.type -= 100;});
}
