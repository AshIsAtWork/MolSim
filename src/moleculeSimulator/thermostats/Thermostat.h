//
// Created by daniel on 29.06.24.
//

#pragma once

#include "models/Model.h"

/**
 * @brief Base class for controling the temperature of the simulated system.
 */

class Thermostat {

    protected:
    /**
     * Paramters used in this thermostat:
     *                                   -model: Model on which this thermostat is applied
     *                                   -initTemperature:  The initial temperature of the system
     *                                   -targetTemperature: Temperature the system should reach and preserve
     *                                   -maxTemperatureChange: The maximal absolute temperature change that is allowed
     *                                    for one application of the thermostat
     *                                   -dimensions: Number of dimensions in which the simulation takes place.
     */
    Model &model;
    double initTemperature;
    double targetTemperature;
    double maxTemperatureChange;
    int dimensions;

public:
    /**
       * @brief Helper method to calculate the current kinetic energy of the system.
       *
       * @return Current kinetic energy of the system.
       */
    virtual double calculateKineticEnergy() = 0;

    /**
     * @brief Helper method to calculate the current temperature of the system.
     *
     * @return Current temperature of the system.
     */
    virtual double calculateTemperature() = 0;

    /**
     * @brief Construct a thermostat for a model.
     *
     * @param model Model on which the thermostat is applied.
     * @param temperatureInit The initial temperature of the system.
     * @param temperatureTarget Temperature the system should reach and preserve
     * @param maxTemperatureChange The maximal absolute temperature change that is allowed
     *                             for one application of the thermostat.
     * @param dimensions Number of dimensions in which the simulation takes place.
     */
    Thermostat(Model &model, double temperatureInit, double temperatureTarget, double maxTemperatureChange,
               int dimensions);

    /**
     * @brief Set the temperature of the system to initTemperature before starting the simultion by applying
     *        Brownian Motion to all particles.
     */
    void initialiseSystem();

    /**
     * @brief Set the temperature of the current system to targetTemperature.
     */
    virtual void setTemperatureOfTheSystemViaVelocityScaling() = 0;

    /**
     * @brief Set the temperature of the current system to targetTemperature, if the change in temperature does not
     *        exceed maxTemperatureChange. If it does, the temperature will only be adjusted by the value of maxTemperatureChange.
     */
    virtual void setTemperatureOfTheSystemViaGradualVelocityScaling() = 0;

    /**
     * Setter for testing purposes
     */
    void setTargetTemperature(double targetTemperature) {
        this->targetTemperature = targetTemperature;
    }

    virtual ~Thermostat() = default;
};



