//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, double sigma, FileHandler::inputFormat inputFormat,
                         FileHandler::outputFormat outputFormat,
                         std::array<std::pair<Side, enumsStructs::BoundaryCondition>, 6> &
                         boundarySettings) : Model(particles, force, deltaT, inputFormat,
                                                  outputFormat),
                                            particles(domainSize, rCutOff),
                                            boundarySettings{boundarySettings} {
    threshold = pow(2.0, 1.0 / 6.0) * sigma;
}

void LinkedCells::processBoundaries() {
    for (auto setting: boundarySettings) {
        switch (setting.second) {
            case BoundaryCondition::outflow: {
                particles.clearHaloCells(setting.first);
            }
            break;
            case BoundaryCondition::reflective: {
                particles.applyToAllBoundryParticles([this](Particle &p, std::array<double, 3>& ghostPosition) {
                    //Add force from an imaginary ghost particle to particle p
                    Particle ghostParticle = p;
                    ghostParticle.setX(ghostPosition);
                    std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
                    p.setF(p.getF() + ghostForce);
                }, setting.first, threshold);
            }
            break;
            case BoundaryCondition::invalid: {
                spdlog::error("Invalid boundary condition was selected. Terminating program!");
                exit(-1);
            };
        }
    }
}

void LinkedCells::step() {
    updateForces();
    processBoundaries();
    updateVelocities();
    updatePositions();
    particles.updateCells();
}
