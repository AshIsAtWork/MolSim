//
// Created by daniel on 29.06.24.
//

#pragma once
#include "Thermostat.h"

/**
 * @brief Class for controling the temperature of a flow.
 */

class FlowThermostat : public Thermostat{
public:

    /**
     * @brief Construct a flow thermostat for a model.
     *
     * @param model Model on which the thermostat is applied.
     * @param temperatureInit The initial temperature of the system.
     * @param temperatureTarget Temperature the system should reach and preserve
     * @param maxTemperatureChange The maximal absolute temperature change that is allowed
     *                             for one application of the thermostat.
     * @param dimensions Number of dimensions in which the simulation takes place.
     */
    FlowThermostat(Model &model, double temperatureInit, double temperatureTarget, double maxTemperatureChange,
               int dimensions);

    /**
    * @brief Helper method to calculate the current kinetic energy of the system. (Fixed particles excluded).
    *
    * @return Current kinetic energy of the system.
    */
    double calculateKineticEnergy() override;

    /**
     * Helper method to calculate the current average velocity of the particles within the system. (Fixed particles excluded).
     *
     * @return Current avg velocity of the system.
     */
    std::array<double, 3> calculateAverageVelocity();

    /**
     * @brief Helper method to calculate the current temperature of the system.
     *
     * @return Current temperature of the system.
     */
    double calculateTemperature() override;

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
