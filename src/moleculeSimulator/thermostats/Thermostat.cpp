//
// Created by daniel on 29.06.24.
//

#include "Thermostat.h"

Thermostat::Thermostat(Model &model, double temperatureInit, double temperatureTarget, double maxTemperatureChange,
                       int dimensions) : model{model}, initTemperature{temperatureInit},
                                         targetTemperature{temperatureTarget},
                                         maxTemperatureChange{maxTemperatureChange}, dimensions{dimensions} {
}

void Thermostat::initialiseSystem() {
    model.particles.applyToEachParticleInDomain([this](Particle &p) {
        if(!p.isFixed()) {
            double f_i = std::sqrt(initTemperature / p.getM());
            p.setV(maxwellBoltzmannDistributedVelocity(f_i, dimensions));
        }
    });
}

