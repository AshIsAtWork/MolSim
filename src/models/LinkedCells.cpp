//
// Created by daniel on 22.05.24.
//

#include "LinkedCells.h"

LinkedCells::LinkedCells(Force &force, double deltaT, std::array<double, 3> domainSize,
                         double rCutOff, FileHandler::inputFormat inputFormat,
                         FileHandler::outputFormat outputFormat, BoundryCondition boundryCondition) :
                         Model(particles, force, deltaT, inputFormat,outputFormat),
                         particles(domainSize, rCutOff), boundryCondition{boundryCondition} {
}

void LinkedCells::processBoundries() {
    switch(boundryCondition) {
        case BoundryCondition::outflow :{
            particles.clearHaloCells();
        }break;
        case BoundryCondition::reflective: {

            for(auto boundry : boundries) {
                particles.applyToAllBoundryParticles([this](Particle& p, std::array<double, 3> ghostPosition) {
               //Add force from an imaginary ghost particle to particle p
                   Particle ghostParticle = p;
                   ghostParticle.setX(ghostPosition);
               std::array<double, 3> ghostForce = force.compute(p, ghostParticle);
               p.setF(p.getF() + ghostForce);
           }, boundry, threshhold);
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
