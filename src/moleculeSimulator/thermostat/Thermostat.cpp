//
// Created by daniel on 08.06.24.
//

#include "Thermostat.h"

#include <boost/math/special_functions/sign.hpp>


Thermostat::Thermostat(Model &model, double temperatureInit, double temperatureTarget,
                       double maxTemperatureChange, int dimensions) : model{model}, initTemperature{temperatureInit},
                                                                      targetTemperature{temperatureTarget},
                                                                      maxTemperatureChange{maxTemperatureChange},
                                                                      dimensions{dimensions} {
}

void Thermostat::initialiseSystem() {
    model.particles.applyToEachParticleInDomain([this](Particle &p) {
        double f_i = std::sqrt(initTemperature / p.getM());
        p.setV(maxwellBoltzmannDistributedVelocity(f_i, dimensions));
    });
}

double Thermostat::calculateKineticEnergy() {
    double eKin = 0;
    model.particles.applyToEachParticleInDomain([&eKin](Particle &p) {
        eKin += p.calculateEKin();
    });
    return eKin;
}

double Thermostat::calculateTemperature() {
    if(model.particles.size() == 0) {
        throw std::invalid_argument("Temperature given when none expected.");
    }
    return 2 * calculateKineticEnergy() / (static_cast<double>(model.particles.size()) * dimensions);
}

void Thermostat::setTemperatureOfTheSystemViaVelocityScaling() {
    double currentTemperature = calculateTemperature();
    if (currentTemperature == 0) {
        throw std::invalid_argument("Temperature Scaling error. Current System temperature is 0.");
    }
    double beta = std::sqrt(targetTemperature / currentTemperature);
    model.particles.applyToEachParticleInDomain([beta](Particle &p) {
        p.setV(beta * p.getV());
    });
}

void Thermostat::setTemperatureOfTheSystemViaGradualVelocityScaling() {
    double currentTemperature = calculateTemperature();
    if (currentTemperature == 0) {
        throw std::invalid_argument("Temperature Scaling error. Current System temperature is 0.");
    }
    double intermediateTemperature = targetTemperature;
    double temperatureChange = targetTemperature - currentTemperature;
    if (std::abs(temperatureChange) > maxTemperatureChange) {
        intermediateTemperature = currentTemperature + boost::math::sign(temperatureChange) * maxTemperatureChange;
    }
    double beta = std::sqrt(intermediateTemperature / currentTemperature);
    model.particles.applyToEachParticleInDomain([beta](Particle &p) {
        p.setV(beta * p.getV());
    });
}
