//
// Created by daniel on 29.05.24.
//

#pragma once
#include <array>

#include "models/LinkedCells.h"

enum class TypeOfForce {gravity, leonardJonesForce};

struct DirectSumSimulationParameters {
    double deltaT;
    double endT;
    double epsilon;
    double sigma;
    TypeOfForce force;
};

struct LinkedCellsSimulationParameters {
    double deltaT;
    double endT;
    double epsilon;
    double sigma;
    TypeOfForce force;
    double rCutOff;
    std::array<double, 3> domainSize;
    std::array<std::pair<LinkedCellsContainer::Side, LinkedCells::BoundryCondition>, 6> boundrySettings;
};