//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, double sigma, FileHandler::outputFormat outputFormat,
                         std::array<std::pair<Side, enumsStructs::BoundaryCondition>, 6> &
                         boundarySettings) : Model(particles, force, deltaT, outputFormat),
                                             particles(domainSize, rCutOff),
                                             boundarySettings{boundarySettings} {
    threshold = pow(2.0, 1.0 / 6.0) * sigma;
    gravityOn = false;
    g = -12.44;
}

void LinkedCells::processBoundaries() {
    for (auto setting: boundarySettings) {
        switch (setting.second) {
            case BoundaryCondition::outflow: {
                particles.clearHaloCells(setting.first);
            }
            break;
            case BoundaryCondition::reflective: {
                particles.applyToAllBoundaryParticles([this](Particle &p, std::array<double, 3> &ghostPosition) {
                    //Add force from an imaginary ghost particle to particle p
                    Particle ghostParticle = p;
                    ghostParticle.setX(ghostPosition);
                    std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
                    p.setF(p.getF() + ghostForce);
                }, setting.first, threshold);
            }
            break;
            case BoundaryCondition::periodic: {

                //Particles that have left the domain will be inserted at the opposite side
                particles.teleportParticlesToOppositeSide(setting.first);
                //Add forces from particles of adjacent boundary cells from the opposite side.
                particles.applyForcesFromOppositeSide(setting.first);
            }break;
            case BoundaryCondition::invalid: {
                spdlog::error("Invalid boundary condition was selected. Terminating program!");
                exit(-1);
            };
        }
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
                }, setting.first, threshold);
            }break;
            case BoundaryCondition::periodic: {
                //Add forces from particles of adjacent boundary cells from the opposite side.
                particles.applyForcesFromOppositeSide(setting.first);
            }break;
            case BoundaryCondition::invalid: {
                spdlog::error("Invalid boundary condition was selected. Terminating program!");
                exit(-1);
            };
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
            }break;
            case BoundaryCondition::invalid: {
                spdlog::error("Invalid boundary condition was selected. Terminating program!");
                exit(-1);
            };
        }
    }


}

void LinkedCells::applyGravity() {
    particles.applyToEachParticleInDomain([this](Particle &p) {
        auto force = p.getF();
        force[1] += p.getM() * g;
        p.setF(force);
    });
}

void LinkedCells::step() {
    updateForces();
    if(gravityOn) {
        applyGravity();
    }
    processBoundaryForces();
    updateVelocities();
    updatePositions();
    particles.updateCells();
    processHaloCells();
}
