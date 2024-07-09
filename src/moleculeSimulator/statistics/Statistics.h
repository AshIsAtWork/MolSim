//
// Created by daniel on 29.06.24.
//
#pragma once

#include "fileHandling/outputWriter/CSVWriter/CSVWriter.h"
#include "particleRepresentation/container/ParticleContainer.h"

/**
 * @brief Class for profiling the nano-scale flow.
 */
class Statistics {
private:
    CSVWriter csvVelocityWriter;
    CSVWriter csvDensityWriter;
    int numberOfBins;


public:
    /**
     * @brief Construct a new statistic object.
     *
     * @param numberOfBins Number of bins into which the x-axis is divided.
     */
    explicit Statistics(int numberOfBins);

    /**
     * @brief Calculate velocity and density profile and write it to two csv file (each profile has its own file).
     *
     * @param particles Particles to profile.
     * @param dimensions Dimensions of the domain {x,y,z}.
     * @param time Current time in the simulation.
     */
    void calculateVelocityAndDensityProfile(ParticleContainer& particles, std::array<double, 3> dimensions, double time);
};

