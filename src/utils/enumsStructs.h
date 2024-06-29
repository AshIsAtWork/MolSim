//
// Created by daniel on 29.05.24.
//

#pragma once

#include <array>

namespace enumsStructs {
    /**
     * Enum to specify the side that can be chosen in some methods.
     */
    enum class Side {
        front, right, back, left, top, bottom
    };

    /**
     * Enum to specify the type of force used in the simulation.
     */
    enum class TypeOfForce {
        gravity, leonardJonesForce, invalid
    };

    /**
     * Enum to specify the type of model used in the simulation.
     */
    enum class TypeOfModel {
        directSum, linkedCells, invalid
    };

    /**
     * Enum to distinguish between different boundary conditions for each side.
     */
    enum class BoundaryCondition {
        outflow, reflective, periodic, invalid
    };

    struct BoundarySet {
        BoundaryCondition front = BoundaryCondition::invalid;
        BoundaryCondition right = BoundaryCondition::invalid;
        BoundaryCondition back = BoundaryCondition::invalid;
        BoundaryCondition left = BoundaryCondition::invalid;
        BoundaryCondition top = BoundaryCondition::invalid;
        BoundaryCondition bottom = BoundaryCondition::invalid;
    };

    /**
     * Struct for passing parameters of the direct sum model.
     */
    struct DirectSumSimulationParameters {
        double deltaT;
        double endT;
        TypeOfForce force;
    };

    /**
     * Struct for passing parameters of the linked cells model.
     */
    struct LinkedCellsSimulationParameters {
        double deltaT;
        double endT;
        TypeOfForce force;
        double rCutOff;
        std::array<double, 3> domainSize;
        BoundarySet boundaryConditions;
    };

    /**
     * Struct containing cuboid specifications.
     */
    struct Cuboid {
        std::array<double, 3> position;
        std::array<unsigned, 3> dimensions;
        double h;
        double mass;
        std::array<double, 3> initVelocity;
        int dimensionsBrownianMotion;
        double brownianMotionAverageVelocity;
        double epsilon;
        double sigma;
        bool fixed;
    };

    /**
     * Struct containing disc specifications.
     */
    struct Disc {
        std::array<double, 3> center;
        std::array<double, 3> initVelocity;
        int N;
        double h;
        double mass;
        int dimensionsBrownianMotion;
        double brownianMotionAverageVelocity;
        double epsilon;
        double sigma;
        bool fixed;
    };

    struct Sphere {
        std::array<double, 3> center;
        std::array<double, 3> initVelocity;
        int N;
        double h;
        double mass;
        int dimensionsBrownianMotion;
        double brownianMotionAverageVelocity;
        double epsilon;
        double sigma;
        bool fixed;
    };

    /**
     * Struct containing particle specifications.
     */
    struct ParticleType {
        std::array<double, 3> x;
        std::array<double, 3> v;
        double m;
        double epsilon;
        double sigma;
    };

    /**
     * Struct containing the specifications of the thermostat.
     */

    struct ThermostatParameters {
        //If set to false, all other parameters are ignored
        bool useThermostat;
        bool initialiseSystemWithBrownianMotion;
        //If set to false, maxTemperatureChange will be ignored
        bool applyScalingGradually;
        double initialTemperature;
        double targetTemperature;
        double maxTemperatureChange;
        int applyAfterHowManySteps;
        int dimensions;
    };

    struct MembraneParameters {
        //Setting
        bool membraneSetting;
        bool pull;
        int pullingActiveUntil;
        std::array<double, 3> pullingForce;
        double r0;
        double k;
        //Membrane itself
        std::array<double, 3> initialVelocity;
        std::array<double, 3> position;
        unsigned N1;
        unsigned N2;
        double mass;
        double h;
        double epsilon;
        double sigma;
    };

    /**
     * Struct for storing all simulation parameters.
     */
    struct SimulationSettings {
        //general
        std::string outputFileName;
        int outputFrequency;
        bool gravityOn;
        std::array<double, 3> gravityVector;

        //thermostat
        ThermostatParameters thermostatParameters;

        //membrane
        MembraneParameters membraneParameters;

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
        std::vector<Sphere> spheres;
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
     * @brief Convert enum value to string.
     *
     * @param force Enum value to convert.
     *
     * @return Corresponding string.
     */
    inline std::string getForce(TypeOfForce &force) {
        static const std::unordered_map<TypeOfForce, std::string> formatMap = {
            {TypeOfForce::gravity, "Gravity"},
            {TypeOfForce::leonardJonesForce, "LeonardJonesForce"}
        };
        auto it = formatMap.find(force);
        return (it != formatMap.end()) ? it->second : "Invalid";
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

    /**
     * @brief Convert enum value to string.
     *
     * @param model Enum value to convert.
     *
     * @return Corresponding string.
     */
    inline std::string getModel(TypeOfModel &model) {
        static const std::unordered_map<TypeOfModel, std::string> formatMap = {
            {TypeOfModel::directSum, "DirectSum"},
            {TypeOfModel::linkedCells, "LinkedCells"}
        };
        auto it = formatMap.find(model);
        return (it != formatMap.end()) ? it->second : "Invalid";
    }

    /**
     * @brief Convert string selection to corresponding enum value.
     *
     * @param selectedBoundaryCondition String to convert.
     *
     * @return Corresponding enum value.
     */
    inline BoundaryCondition setBoundaryCondition(const std::string &selectedBoundaryCondition) {
        static const std::unordered_map<std::string, BoundaryCondition> formatMap = {
            {"Reflective", BoundaryCondition::reflective},
            {"Outflow", BoundaryCondition::outflow},
            {"Periodic", BoundaryCondition::periodic}
        };
        auto it = formatMap.find(selectedBoundaryCondition);
        return (it != formatMap.end()) ? it->second : BoundaryCondition::invalid;
    }

    /**
     * @brief Convert enum value to string.
     *
     * @param boundaryCondition Enum value to convert.
     *
     * @return Corresponding string.
     */
    inline std::string getBoundaryCondition(BoundaryCondition &boundaryCondition) {
        static const std::unordered_map<BoundaryCondition, std::string> formatMap = {
            {BoundaryCondition::reflective, "Reflective"},
            {BoundaryCondition::outflow, "Outflow"},
            {BoundaryCondition::periodic, "Periodic"}
        };
        auto it = formatMap.find(boundaryCondition);
        return (it != formatMap.end()) ? it->second : "Invalid";
    }
}
