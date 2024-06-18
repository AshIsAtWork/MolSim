//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, FileHandler::outputFormat outputFormat,
                         BoundarySet boundaryConditions, bool gravityOn, double g) : Model(particles, force, deltaT,
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
            }
            break;
            case BoundaryCondition::periodic: {
                //Add forces from particles of adjacent boundary cells from the opposite side.
                particles.applyForcesFromOppositeSide(setting.first);
            }
            break;
            case BoundaryCondition::invalid: {
                throw std::invalid_argument("Invalid Boundary Condition was selected.");
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
            }
            break;
            case BoundaryCondition::invalid: {
                throw std::invalid_argument("Invalid Boundary condition was selected.");
            };
        }
    }
}

void LinkedCells::step() {
    particles.updateForcesAtOnce();
    if (gravityOn) {
        applyGravity();
    }
    processBoundaryForces();
    updateVelocities();
    updatePositions();
    particles.updateCells();
    processHaloCells();
}
