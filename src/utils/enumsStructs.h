//
// Created by daniel on 29.05.24.
//

#pragma once
#include <array>

namespace enumsStructs {
    /**
     * Enum to specify the side that can be chosen in some methods.
     */
    enum class Side { front, right, back, left, top, bottom };

    /**
     * Enum to specify the type of force used in the simulation.
     */
    enum class TypeOfForce { gravity, leonardJonesForce, invalid };

    /**
     * Enum to specify the type of model used in the simulation.
     */
    enum class TypeOfModel { directSum, linkedCells, invalid };

    /**
     * Enum to distinguish between different boundry conditions for each side.
     */
    enum class BoundaryCondition { outflow, reflective, invalid };

    /**
     * Struct for passing parameters of the direct sum model.
     */
    struct DirectSumSimulationParameters {
        double deltaT;
        double endT;
        double epsilon;
        double sigma;
        TypeOfForce force;
    };

    /*
     * Struct for passing parameters of the linked cells model
     */
    struct LinkedCellsSimulationParameters {
        double deltaT;
        double endT;
        double epsilon;
        double sigma;
        TypeOfForce force;
        double rCutOff;
        std::array<double, 3> domainSize;
        std::array<std::pair<Side, BoundaryCondition>, 6> boundarySettings;
    };

    struct Cuboid {
        std::array <double, 3> position;
        std::array <unsigned, 3> dimensions;
        double h;
        double mass;
        std::array <double, 3> initVelocity;
        int dimensionsBrownianMotion;
        double brownianMotionAverageVelocity;
    };

    struct Disc {
        std::array <double, 3> center;
        std::array <double, 3> initVelocity;
        int N;
        double h;
        double mass;
        int dimensionsBrownianMotion;
        double brownianMotionAverageVelocity;
    };

    struct ParticleType {
        std::array<double, 3> x;
        std::array<double, 3> v;
        double m;
    };


    struct SimulationSettings {
        //general
        std::string outputFileName;
        int outputFrequency;

        //model to use in the simulation
        TypeOfModel model;

        //Parameters for DirectSumModel
        DirectSumSimulationParameters parametersDirectSum;

        //Parameters for LinkedCellModel
        LinkedCellsSimulationParameters parametersLinkedCells;

        //Particles and objects of particles
        std::vector<Cuboid> cuboids;
        std::vector<Disc> discs;
        std::vector<ParticleType> particles;
    };

    /**
     * @brief Convert string selection to enum value.
     *
     * @param selectedForce String to convert.
     *
     * @return Corresponding enum value.
     */
    inline TypeOfForce setForce(const std::string &selectedForce) {
        static const std::unordered_map<std::string, TypeOfForce> formatMap = {
            {"Gravity", TypeOfForce::gravity},
            {"LeonardJonesForce", TypeOfForce::leonardJonesForce}
        };

        auto it = formatMap.find(selectedForce);
        return (it != formatMap.end()) ? it->second : TypeOfForce::invalid;
    }

    /**
     * @brief Convert string selection to corresponding enum value.
     *
     * @param selectedModel String to convert.
     *
     * @return Corresponding enum value.
     */
    inline TypeOfModel setModel(const std::string &selectedModel) {
        static const std::unordered_map<std::string, TypeOfModel> formatMap = {
            {"DirectSum", TypeOfModel::directSum},
            {"LinkedCells", TypeOfModel::linkedCells}
        };

        auto it = formatMap.find(selectedModel);
        return (it != formatMap.end()) ? it->second : TypeOfModel::invalid;
    }

    inline BoundaryCondition setBoundaryCondition(const std::string& selectedBoundaryCondition) {
        static const std::unordered_map<std::string, BoundaryCondition> formatMap = {
            {"Reflective", BoundaryCondition::reflective},
            {"Outflow", BoundaryCondition::outflow}
        };
        auto it = formatMap.find(selectedBoundaryCondition);
        return (it != formatMap.end()) ? it->second : BoundaryCondition::invalid;
    }

}
