//
// Created by daniel on 29.05.24.
//

#pragma once
#include <array>

#include "models/LinkedCells.h"

enum class TypeOfForce {gravity, leonardJonesForce, invalid};

enum class TypeOfModel {directSum, linkedCells, invalid};

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
    std::array<std::pair<LinkedCellsContainer::Side, LinkedCells::BoundaryCondition>, 6> boundarySettings;
};

inline TypeOfForce setForce(const std::string &selectedForce) {
    static const std::unordered_map<std::string, TypeOfForce> formatMap = {
            {"gravity", TypeOfForce::gravity},
            {"ljf", TypeOfForce::leonardJonesForce}
    };

    auto it = formatMap.find(selectedForce);
    return (it != formatMap.end()) ? it->second : TypeOfForce::invalid;
}

inline TypeOfModel setModel(const std::string &selectedModel) {
    static const std::unordered_map<std::string, TypeOfModel> formatMap = {
            {"ds", TypeOfModel::directSum},
            {"lc", TypeOfModel::linkedCells}
    };

    auto it = formatMap.find(selectedModel);
    return (it != formatMap.end()) ? it->second : TypeOfModel::invalid;
}