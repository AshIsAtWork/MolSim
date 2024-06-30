//
// Created by daniel on 29.06.24.
//
#pragma once
#include <boost/throw_exception.hpp>

#include "fileHandling/outputWriter/CSVWriter/CSVWriter.h"
#include "particleRepresentation/container/ParticleContainer.h"

class Statistics {
private:
    CSVWriter csvWriter;
    int numberOfBins;


public:
    Statistics(int numberOfBins);

    void calculateVelocityAndDensityProfile(ParticleContainer& particles, double lengthX, double time);
};

