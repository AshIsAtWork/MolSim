//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, FileHandler::inputFormat inputFormat,
                         FileHandler::outputFormat outputFormat,
                         std::array<std::pair<LinkedCellsContainer::Side, BoundryCondition>, 6> &
                         boundrySettings) : Model(particles, force, deltaT, inputFormat,
                                                  outputFormat),
                                            particles(domainSize, rCutOff),
                                            boundrySettings{boundrySettings} {
}

void LinkedCells::processBoundries() {
    for (auto setting: boundrySettings) {
        switch (setting.second) {
            case BoundryCondition::outflow: {
                particles.clearHaloCells(setting.first);
            }
            break;
            case BoundryCondition::reflective: {
                particles.applyToAllBoundryParticles([this](Particle &p, std::array<double, 3> ghostPosition) {
                    //Add force from an imaginary ghost particle to particle p
                    Particle ghostParticle = p;
                    ghostParticle.setX(ghostPosition);
                    std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
                    p.setF(p.getF() + ghostForce);
                }, setting.first, threshhold);
            }
        }
    }
}

void LinkedCells::step() {
    updateForces();
    processBoundries();
    updateVelocities();
    updatePositions();
    particles.updateCells();
}
