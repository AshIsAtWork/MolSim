//
// Created by daniel on 22.05.24.
//
#include "Model.h"

#include "moleculeSimulator/particleGeneration/ParticleGenerator.h"

Model::Model(ParticleContainer &particles, Force &force, double deltaT,  FileHandler::inputFormat inputFormat,
    FileHandler::outputFormat outputFormat) : inputFormat{inputFormat}, outputFormat{outputFormat}, particles{particles}, force{force},
                                                                          deltaT{deltaT} {
}

void Model::updateForces() const {
    //Before calculating the new forces, the current forces have to be reset.
    particles.applyToEachParticleInDomain([](Particle &p) {
        p.resetForce();
    });
    //Calculate new forces using Newtons third law of motion
    particles.applyToAllUniquePairsInDomain([this](Particle &p_i, Particle &p_j) {
        auto f_ij{force.compute(p_i, p_j)};
        p_i.setF(p_i.getF() + f_ij);
        p_j.setF(p_j.getF() - f_ij);
    });
}

void Model::updatePositions() const{
    particles.applyToEachParticleInDomain([this](Particle &p) {
        p.setX(p.getX() + deltaT * p.getV() + ((deltaT * deltaT) / (2.0 * p.getM())) * p.getOldF());
    });
}

void Model::updateVelocities() const{
    particles.applyToEachParticleInDomain([this](Particle &p) {
        p.setV(p.getV() + (deltaT / (2 * p.getM())) * (p.getOldF() + p.getF()));
    });
}

void Model::plot(int iteration, std::string& baseName) {
    fileHandler.writeToFile(particles, iteration, outputFormat, baseName);
}

void Model::addCuboid(const std::array<double, 3> &position, unsigned N1, unsigned N2,
                      unsigned N3, double h, double mass, const std::array<double, 3> &initVelocity, int dimensions, double brownianMotionAverageVelocity) {
    ParticleGenerator::generateCuboid(particles, position, N1, N2, N3, h, mass, initVelocity, dimensions, brownianMotionAverageVelocity);
}

void Model::addDisc(const std::array<double, 3> &center,
    const std::array<double, 3> &initVelocity, int N, double h, double mass, int dimensions, double brownianMotionAverageVelocity) {
    ParticleGenerator::generateDisc(particles, center,initVelocity, N, h, mass, dimensions, brownianMotionAverageVelocity);
}

void Model::addParticle(Particle &p) {
    particles.add(p);
}

void Model::addViaFile(std::string &filepath) {
    FileHandler::readFile(particles,filepath, inputFormat);
}
