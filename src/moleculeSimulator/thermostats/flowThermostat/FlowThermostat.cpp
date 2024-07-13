//
// Created by daniel on 29.06.24.
//

#include "FlowThermostat.h"

FlowThermostat::FlowThermostat(Model &model, double temperatureInit, double temperatureTarget,
                               double maxTemperatureChange, int dimensions) : Thermostat{
    model, temperatureInit, temperatureTarget, maxTemperatureChange, dimensions
} {
}

double FlowThermostat::calculateKineticEnergy() {
    double eKin = 0;
    auto avgVelocity = calculateAverageVelocity();
    model.particles.applyToEachParticleInDomain([&eKin, &avgVelocity](Particle &p) {
        if(!p.isFixed()) {
            eKin += p.calculateEKinFlow(avgVelocity);
        }
    });
    return eKin;
}

double FlowThermostat::calculateTemperature() {
    int numberNonFixedParticles = 0;
    model.particles.applyToEachParticleInDomain([&numberNonFixedParticles](Particle& p) {
        if(!p.isFixed()) {
            numberNonFixedParticles++;
        }
    });
    if(numberNonFixedParticles == 0) {
        throw std::invalid_argument("Temperature for 0 particles is not defined!");
    }
    return 2 * calculateKineticEnergy() / (numberNonFixedParticles * dimensions);
}

std::array<double, 3> FlowThermostat::calculateAverageVelocity() {
    int numberNonFixedParticles = 0;
    std::array<double, 3> sum = {0,0,0};
    model.particles.applyToEachParticleInDomain([&sum, &numberNonFixedParticles](Particle& p) {
        if(!p.isFixed()) {
            sum = sum + p.getV();
            numberNonFixedParticles++;
        }
    });
    if(numberNonFixedParticles == 0) {
        throw std::invalid_argument("Average velocity for 0 particles is not defined!");
    }
    return (1.0 / numberNonFixedParticles) * sum;
}

void FlowThermostat::setTemperatureOfTheSystemViaVelocityScaling() {
    double currentTemperature = calculateTemperature();
    if (currentTemperature == 0) {
        throw std::invalid_argument("Temperature Scaling error. Current System temperature is 0.");
    }
    double beta = std::sqrt(targetTemperature / currentTemperature);
    auto avgVelocity = calculateAverageVelocity();
    model.particles.applyToEachParticleInDomain([beta, avgVelocity](Particle &p) {
        if(!p.isFixed()) {
            p.setV(avgVelocity + beta * (p.getV() - avgVelocity));
        }
    });
}

void FlowThermostat::setTemperatureOfTheSystemViaGradualVelocityScaling() {
    double currentTemperature = calculateTemperature();
    auto avgVelocity = calculateAverageVelocity();
    if (currentTemperature == 0) {
        throw std::invalid_argument("Temperature Scaling error. Current System temperature is 0.");
    }
    double intermediateTemperature = targetTemperature;
    double temperatureChange = targetTemperature - currentTemperature;
    double signTemperatureChange = temperatureChange > 0 ? 1 : temperatureChange < 0 ? -1 : 0;
    if (std::abs(temperatureChange) > maxTemperatureChange) {
        intermediateTemperature = currentTemperature + signTemperatureChange * maxTemperatureChange;
    }
    double beta = std::sqrt(intermediateTemperature / currentTemperature);
    model.particles.applyToEachParticleInDomain([beta, avgVelocity](Particle &p) {
        if(!p.isFixed()) {
            p.setV(avgVelocity + beta * (p.getV() - avgVelocity));
        }
    });
}
