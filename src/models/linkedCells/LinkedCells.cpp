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
        ParticleGenerator::generateMembrane(particles, membraneParameters.position,
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
                    if(!p.isFixed()) {
                        Particle ghostParticle = p;
                        ghostParticle.setX(ghostPosition);
                        std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
                        p.setF(p.getF() + ghostForce);
                    }
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
            break;

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

void LinkedCells::pullMarkedParticles() {
    particles.applyToEachParticleInDomain([this](Particle& p) {
        if(p.isMarked()) {
            p.setF(p.getF() + pullingForce);
        }
    });
}

void LinkedCells::updateForcesMembrane() {
    //Before calculating the new forces, the current forces have to be reset.
    particles.applyToEachParticleInDomain([](Particle &p) {
        p.resetForce();
    });
    //Calculate new forces using Newtons third law of motion
    particles.applyToAllUniquePairsInDomain([this](Particle &p_i, Particle &p_j) {
        //Apply harmonic forces
        //1. Check, if they are direct neighbors
        if(p_i.isDirectNeighbor(p_j)) {
            auto f = forceBetweenDirectNeighborsInMembrane->compute(p_i,p_j);
            p_i.setF(p_i.getF() + f);
            p_j.setF(p_j.getF() - f);
        }
        //2. Check, if they are diagonal neighbors
        else if(p_i.isDiagonalNeighbor(p_j)) {
            auto f = forceBetweenDiagonalNeighborsInMembrane->compute(p_i,p_j);
            p_i.setF(p_i.getF() + f);
            p_j.setF(p_j.getF() - f);
        }

        //If they are no direct or diagonal neighbors than truncated Lennard-Jones force is applied

        else if (ArrayUtils::L2Norm(p_i.getX() - p_j.getX()) <= std::pow(2.0, 1.0 / 6.0) * p_i.getSigma()) {
            auto f_ij{force.compute(p_i, p_j)};
            p_i.setF(p_i.getF() + f_ij);
            p_j.setF(p_j.getF() - f_ij);
        }
    });
}

void LinkedCells::step(int iteration) {
    if (membraneSetting) {
        updateForcesMembrane();
        if (pull && iteration <= pullingActiveUntil) {
            pullMarkedParticles();
        }
    } else {
        updateForces();
    }
    if (gravityOn) {
        applyGravity();
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
       updateForcesMembrane();
        if (pull) {
            pullMarkedParticles();
        }
    }else {
        updateForces();
    }
    if (gravityOn) {
        applyGravity();
    }
    processBoundaryForces();
}
