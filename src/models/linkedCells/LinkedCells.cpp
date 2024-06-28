//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

#include <iostream>

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, FileHandler::outputFormat outputFormat,
                         BoundarySet boundaryConditions, bool gravityOn, std::array<double, 3> g,
                         MembraneParameters membraneParameters) : Model(particles, force, deltaT,
                                                                        outputFormat, gravityOn, g),
                                                                  particles(domainSize, rCutOff, boundaryConditions) {
    std::pair<Side, BoundaryCondition> cFront{Side::front, boundaryConditions.front};
    boundarySettings.push_back(cFront);
    std::pair<Side, BoundaryCondition> cRight{Side::right, boundaryConditions.right};
    boundarySettings.push_back(cRight);
    std::pair<Side, BoundaryCondition> cBack{Side::back, boundaryConditions.back};
    boundarySettings.push_back(cBack);
    std::pair<Side, BoundaryCondition> cLeft{Side::left, boundaryConditions.left};
    boundarySettings.push_back(cLeft);
    if (!particles.isTwoD()) {
        std::pair<Side, BoundaryCondition> cTop{Side::top, boundaryConditions.top};
        boundarySettings.push_back(cTop);
        std::pair<Side, BoundaryCondition> cBottom{Side::bottom, boundaryConditions.bottom};
        boundarySettings.push_back(cBottom);
    }

    //Configure membrane

    if (membraneParameters.membraneSetting) {
        //Configure membrane setting
        membraneSetting = true;
        pull = membraneParameters.pull;
        pullingActiveUntil = membraneParameters.pullingActiveUntil;
        pullingForce = membraneParameters.pullingForce;
        forceBetweenDirectNeighborsInMembrane = std::make_unique<HarmonicForce>(
            membraneParameters.k, membraneParameters.r0);
        forceBetweenDiagonalNeighborsInMembrane = std::make_unique<HarmonicForce>(
            membraneParameters.k, membraneParameters.r0 * std::sqrt(2));
        //Generate membrane
        auto marking = [](unsigned x, unsigned y) {
            //Particle (17,24)
             if (x == 16 and y == 23) {
                 return true;
             }
             //Particle (17,25)
             if (x == 16 and y == 24) {
                 return true;
             }
             //Particle (18,24)
             if (x == 17 and y == 23) {
                 return true;
             }
             //Particle (18,25)
             if (x == 17 and y == 24) {
                 return true;
             }
            return false;
        };
        particlesToPull = ParticleGenerator::generateMembrane(particles, membraneParameters.position,
                                                              membraneParameters.N1, membraneParameters.N2,
                                                              membraneParameters.h, membraneParameters.mass,
                                                              membraneParameters.initialVelocity, marking,
                                                              membraneParameters.epsilon, membraneParameters.sigma);
    } else {
        membraneSetting = false;
        pull = false;
        pullingActiveUntil = 0;
        pullingForce = {0, 0, 0};
    }
}

void LinkedCells::processBoundaryForces() {
    //Periodic and reflective boundaries apply forces on the particles
    for (auto setting: boundarySettings) {
        switch (setting.second) {
            case BoundaryCondition::outflow: //Outflow boundaries do not apply forces
                break;
            case BoundaryCondition::reflective: {
                particles.applyToAllBoundaryParticles([this](Particle &p, std::array<double, 3> &ghostPosition) {
                    //Add force from an imaginary ghost particle to particle p
                    Particle ghostParticle = p;
                    ghostParticle.setX(ghostPosition);
                    std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
                    p.setF(p.getF() + ghostForce);
                }, setting.first);
            };
                break;
            case BoundaryCondition::periodic: {
                //Add forces from particles of adjacent boundary cells from the opposite side.
                particles.applyForcesFromOppositeSide(setting.first);
            }
            break;
            case BoundaryCondition::invalid: {
                throw std::invalid_argument("Invalid Boundary Condition was selected.");
            }
        }
    }
}

void LinkedCells::processHaloCells() {
    for (auto setting: boundarySettings) {
        switch (setting.second) {
            case BoundaryCondition::outflow: {
                //Delete particles from outflow halo cells.
                particles.clearHaloCells(setting.first);
            }

            case BoundaryCondition::reflective:
            case BoundaryCondition::periodic: {
                //For reflective boundaries and  periodic, particles will be teleported to the other side.
                //(We include here reflective boundaries to handle edges and corners corretly, when both conditions are mixed)
                particles.teleportParticlesToOppositeSide(setting.first);
            }
            break;
            case BoundaryCondition::invalid: {
                throw std::invalid_argument("Invalid Boundary condition was selected.");
            };
        }
    }
}

void LinkedCells::pullSelectedParticles() {
    for (const auto &p: particlesToPull) {
        p->setF(p->getF() + pullingForce);
    }
}

void LinkedCells::applyForcesBetweenNeighborsInMembrane() {
    particles.applyToEachParticleInDomain([this](Particle &p) {
        p.applyToDirectNeighborsAndSelf([this](Particle& self, Particle& neighbor) {
            auto f = forceBetweenDirectNeighborsInMembrane->compute(self, neighbor);
            self.setF(self.getF() + f);
            neighbor.setF(neighbor.getF() - f);
            // spdlog::debug("Calculate force between direct neighbors {} and {}", self.getType(), neighbor.getType());
            // spdlog::debug("{} applies to direct neighbor {} force {}", self.getType(), neighbor.getType(), ArrayUtils::to_string(f));
            // spdlog::debug("{} applies to direct neighbor {} force {}", neighbor.getType(), self.getType(), ArrayUtils::to_string((-1)*f));
        });
        p.applyToDiagonalNeighborsAndSelf([this](Particle& self, Particle& neighbor) {
            auto f = forceBetweenDiagonalNeighborsInMembrane->compute(self, neighbor);
            // spdlog::debug("Calculate force between diagonal neighbors {} and {}", self.getType(), neighbor.getType());
            // spdlog::debug("{} applies to diagonal neighbor {} force {}", self.getType(), neighbor.getType(), ArrayUtils::to_string(f));
            // spdlog::debug("{} applies to diagonal neighbor {} force {}", neighbor.getType(), self.getType(), ArrayUtils::to_string((-1)*f));
            self.setF(self.getF() + f);
            neighbor.setF(neighbor.getF() - f);
        });
    });
}

void LinkedCells::updateForcesTruncated() {
    //Before calculating the new forces, the current forces have to be reset.
    particles.applyToEachParticleInDomain([](Particle &p) {
        p.resetForce();
    });
    //Calculate new forces using Newtons third law of motion
    particles.applyToAllUniquePairsInDomain([this](Particle &p_i, Particle &p_j) {
        //Forces are only applied, if they are repulsive and if particles are not neighbors.
        if (ArrayUtils::L2Norm(p_i.getX() - p_j.getX()) <= pow(2.0, 1.0 / 6.0) * p_i.getSigma() and !Particle::areNeighbors(&p_i, &p_j)) {
            auto f_ij{force.compute(p_i, p_j)};
            p_i.setF(p_i.getF() + f_ij);
            p_j.setF(p_j.getF() - f_ij);
        }
    });
}

void LinkedCells::step(int iteration) {
    if (membraneSetting) {
        updateForcesTruncated();
        applyForcesBetweenNeighborsInMembrane();
        if (pull && iteration <= pullingActiveUntil) {
            pullSelectedParticles();
        }
    } else {
        updateForces();
    }
    if (gravityOn) {
        //applyGravity();
    }
    processBoundaryForces();
    updateVelocities();
    updatePositions();
    particles.updateCells();
    processHaloCells();
}

void LinkedCells::updateForcesOptimized() {
    //Before calculating the new forces, the current forces have to be reset.
    particles.applyToEachParticleInDomain([](Particle &p) {
        p.resetForce();
    });
    //Calculate new forces using Newtons third law of motion
    particles.applyToAllUniquePairsInDomainOptimized(
        [this](Particle &p_i, Particle &p_j, std::array<double, 3> difference, double distance) {
            auto f_ij{force.computeOptimized(p_i, p_j, difference, distance)};
            p_i.setF(p_i.getF() + f_ij);
            p_j.setF(p_j.getF() - f_ij);
        });
}

void LinkedCells::initializeForces() {
    if(membraneSetting) {
        updateForcesTruncated();
        applyForcesBetweenNeighborsInMembrane();
    }else {
        updateForces();
    }
}
