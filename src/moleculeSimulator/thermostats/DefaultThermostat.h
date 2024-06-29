//
// Created by daniel on 08.06.24.
//

#pragma once
#include "Thermostat.h"
#include "models/Model.h"

/**
 * @brief Class for controling the temperature of the simulated system.
 */
class DefaultThermostat : public Thermostat{

public:

    /**
     * @brief Helper method to calculate the current kinetic energy of the system.
     *
     * @return Current kinetic energy of the system.
     */
     double calculateKineticEnergy() override;

    /**
     * @brief Helper method to calculate the current temperature of the system.
     *
     * @return Current temperature of the system.
     */
    double calculateTemperature() override;

    /**
     * @brief Construct a default thermostat for a model.
     *
     * @param model Model on which the thermostat is applied.
     * @param temperatureInit The initial temperature of the system.
     * @param temperatureTarget Temperature the system should reach and preserve
     * @param maxTemperatureChange The maximal absolute temperature change that is allowed
     *                             for one application of the thermostat.
     * @param dimensions Number of dimensions in which the simulation takes place.
     */
    DefaultThermostat(Model &model, double temperatureInit, double temperatureTarget, double maxTemperatureChange,
               int dimensions);

    /**
     * @brief Set the temperature of the current system to targetTemperature.
     */
    void setTemperatureOfTheSystemViaVelocityScaling() override;

    /**
     * @brief Set the temperature of the current system to targetTemperature, if the change in temperature does not
     *        exceed maxTemperatureChange. If it does, the temperature will only be adjusted by the value of maxTemperatureChange.
     */
    void setTemperatureOfTheSystemViaGradualVelocityScaling() override;
};
